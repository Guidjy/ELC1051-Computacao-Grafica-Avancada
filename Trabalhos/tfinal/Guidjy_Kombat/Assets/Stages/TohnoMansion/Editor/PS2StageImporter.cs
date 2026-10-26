using System.IO;
using UnityEditor;
using UnityEditor.AssetImporters;
using UnityEngine;

// Automatically configures the Tohno Mansion FBX, its textures and materials on import.
public class PS2StageImporter : AssetPostprocessor
{
    const string Folder = "Stages/TohnoMansion/";
    bool InStage => assetPath.Contains(Folder);

    void OnPreprocessModel()
    {
        if (!InStage) return;
        var mi = (ModelImporter)assetImporter;
        mi.globalScale = 1f;
        mi.useFileScale = true;
        mi.importCameras = false;
        mi.importLights = false;
        mi.importBlendShapes = false;
        mi.importAnimation = false;
        mi.animationType = ModelImporterAnimationType.None;
        mi.importNormals = ModelImporterNormals.Import;
        mi.materialImportMode = ModelImporterMaterialImportMode.ImportViaMaterialDescription;
        mi.materialLocation = ModelImporterMaterialLocation.InPrefab;
        mi.addCollider = false;
    }

    // Removes the pillar nearest the camera on each side of the hall. All pillars share one
    // mesh ("Pillars"), so on each side we find the triangle closest to the camera marker and
    // drop every triangle of that column (pillar rows are ~5 m apart, columns < 1 m wide).
    void OnPostprocessModel(GameObject root)
    {
        if (!InStage) return;
        Transform cam = null;
        foreach (var t in root.GetComponentsInChildren<Transform>(true))
            if (t.name == "MK_CameraStart") cam = t;
        bool found = false;
        foreach (var mf in root.GetComponentsInChildren<MeshFilter>(true))
        {
            if (!mf.name.StartsWith("Pillars") || mf.sharedMesh == null) continue;
            found = true;
            var mesh = mf.sharedMesh;
            var toRoot = root.transform.worldToLocalMatrix * mf.transform.localToWorldMatrix;
            var verts = mesh.vertices;
            for (int i = 0; i < verts.Length; i++) verts[i] = toRoot.MultiplyPoint3x4(verts[i]);
            var camPos = cam != null ? root.transform.InverseTransformPoint(cam.position) : new Vector3(0f, 0f, 1e4f);

            var bmin = verts[0]; var bmax = verts[0];
            foreach (var v in verts) { bmin = Vector3.Min(bmin, v); bmax = Vector3.Max(bmax, v); }
            float radius = 0.15f * Mathf.Max(bmax.x - bmin.x, bmax.z - bmin.z);
            float midX = (bmin.x + bmax.x) * 0.5f;

            var tris = new int[mesh.subMeshCount][];
            for (int s = 0; s < tris.Length; s++) tris[s] = mesh.GetTriangles(s);
            Vector3 Centroid(int[] tr, int t) => (verts[tr[t]] + verts[tr[t + 1]] + verts[tr[t + 2]]) / 3f;
            float Flat(Vector3 a, Vector3 b) => new Vector2(a.x - b.x, a.z - b.z).magnitude;

            // Nearest triangle to the camera on each side (0 = left, 1 = right).
            var nearest = new Vector3[2];
            var best = new[] { float.MaxValue, float.MaxValue };
            foreach (var tr in tris)
                for (int t = 0; t < tr.Length; t += 3)
                {
                    var c = Centroid(tr, t); int side = c.x < midX ? 0 : 1;
                    float d = Flat(c, camPos);
                    if (d < best[side]) { best[side] = d; nearest[side] = c; }
                }

            int removed = 0;
            for (int s = 0; s < tris.Length; s++)
            {
                var kept = new System.Collections.Generic.List<int>(tris[s].Length);
                for (int t = 0; t < tris[s].Length; t += 3)
                {
                    var c = Centroid(tris[s], t); int side = c.x < midX ? 0 : 1;
                    if (best[side] < float.MaxValue && Flat(c, nearest[side]) < radius) { removed++; continue; }
                    kept.Add(tris[s][t]); kept.Add(tris[s][t + 1]); kept.Add(tris[s][t + 2]);
                }
                mesh.SetTriangles(kept, s);
            }
            mesh.RecalculateBounds();
            Debug.Log("[PS2Stage] Removed front pillars: " + removed + " triangles cut from " + mf.name +
                      " (camera marker " + (cam != null ? "found" : "missing") + ").");
        }
        if (!found) Debug.LogWarning("[PS2Stage] No 'Pillars' mesh found - front pillars not removed.");
    }

    void OnPreprocessTexture()
    {
        if (!InStage) return;
        var ti = (TextureImporter)assetImporter;
        ti.textureType = TextureImporterType.Default;
        ti.sRGBTexture = true;
        ti.mipmapEnabled = true;
        ti.filterMode = FilterMode.Bilinear;          // PS2 = bilinear, low-res
        ti.maxTextureSize = 512;
        ti.textureCompression = TextureImporterCompression.Uncompressed;
        ti.wrapMode = assetPath.Contains("T_Glow") ? TextureWrapMode.Clamp : TextureWrapMode.Repeat;
    }

    void OnPreprocessMaterialDescription(MaterialDescription description, Material material, AnimationClip[] clips)
    {
        if (!InStage) return;
        bool glow = description.materialName.EndsWith("_Glow");
        var shader = Shader.Find(glow ? "PS2Stage/Glow" : "PS2Stage/VertexLit");
        if (shader == null)
        {
            Debug.LogWarning("[PS2Stage] Shaders not compiled yet - use Tools > PS2 Stage > Reimport Tohno Mansion.");
            return;
        }
        material.shader = shader;
        if (description.TryGetProperty("DiffuseColor", out TexturePropertyDescription tex) && tex.texture != null)
            material.SetTexture("_MainTex", tex.texture);
        else if (glow)
        {
            // The FBX glow materials carry no texture link, so without this they sample
            // the shader's default white and render as solid squares.
            var glowPath = Path.GetDirectoryName(assetPath).Replace('\\', '/') + "/Textures/T_Glow.png";
            context.DependsOnArtifact(glowPath);
            var glowTex = AssetDatabase.LoadAssetAtPath<Texture2D>(glowPath);
            if (glowTex != null) material.SetTexture("_MainTex", glowTex);
            else Debug.LogWarning("[PS2Stage] " + glowPath + " not found - glow sprites will render as squares.");
        }
        if (description.materialName == "M_Halo_Glow") material.SetFloat("_Intensity", 0.9f);
    }

    [MenuItem("Tools/PS2 Stage/Reimport Tohno Mansion")]
    static void Reimport()
    {
        // Textures first, so the clamp/size settings above apply and T_Glow exists for the model.
        foreach (var guid in AssetDatabase.FindAssets("t:Texture2D"))
        {
            var p = AssetDatabase.GUIDToAssetPath(guid);
            if (p.Contains(Folder)) AssetDatabase.ImportAsset(p, ImportAssetOptions.ForceUpdate);
        }
        foreach (var guid in AssetDatabase.FindAssets("TohnoMansion t:Model"))
        {
            var p = AssetDatabase.GUIDToAssetPath(guid);
            if (p.Contains(Folder)) AssetDatabase.ImportAsset(p, ImportAssetOptions.ForceUpdate);
        }
    }

    [MenuItem("Tools/PS2 Stage/Add Tohno Mansion To Scene")]
    static void AddToScene()
    {
        GameObject model = null;
        foreach (var guid in AssetDatabase.FindAssets("TohnoMansion t:Model"))
        {
            var p = AssetDatabase.GUIDToAssetPath(guid);
            if (p.Contains(Folder)) model = AssetDatabase.LoadAssetAtPath<GameObject>(p);
        }
        if (model == null) { Debug.LogError("[PS2Stage] TohnoMansion.fbx not found."); return; }

        var stage = (GameObject)PrefabUtility.InstantiatePrefab(model);
        stage.name = "TohnoMansion_Stage";
        foreach (var t in stage.GetComponentsInChildren<Transform>())
            if (!t.name.StartsWith("MK_")) t.gameObject.isStatic = true;
        Undo.RegisterCreatedObjectUndo(stage, "Add Tohno Mansion");

        var cam = Camera.main;
        if (cam == null)
        {
            var go = new GameObject("Main Camera", typeof(Camera));
            go.tag = "MainCamera";
            cam = go.GetComponent<Camera>();
            Undo.RegisterCreatedObjectUndo(go, "Add Camera");
        }
        cam.clearFlags = CameraClearFlags.SolidColor;
        cam.backgroundColor = new Color(0.02f, 0.025f, 0.06f);
        cam.nearClipPlane = 0.1f; cam.farClipPlane = 120f;
        var rig = cam.GetComponent<FightingStageCamera>();
        if (rig == null) rig = Undo.AddComponent<FightingStageCamera>(cam.gameObject);
        rig.stageRoot = stage.transform;
        rig.Setup();

        // A touch of distance fog for the moody far end (Built-in pipeline).
        RenderSettings.fog = true;
        RenderSettings.fogMode = FogMode.Linear;
        RenderSettings.fogColor = new Color(0.04f, 0.05f, 0.11f);
        RenderSettings.fogStartDistance = 22f;
        RenderSettings.fogEndDistance = 70f;
        Selection.activeGameObject = stage;
    }
}
