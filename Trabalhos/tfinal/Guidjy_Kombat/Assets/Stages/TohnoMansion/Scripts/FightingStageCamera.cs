using UnityEngine;

/// Side-view fighting game camera for the Tohno Mansion stage.
/// The camera only slides along the pan rail defined by the MK_CameraPanLeft / MK_CameraPanRight
/// markers exported from Blender, following the midpoint of the two fighters.
[ExecuteAlways]
[RequireComponent(typeof(Camera))]
public class FightingStageCamera : MonoBehaviour
{
    public Transform stageRoot;
    public Transform player1;
    public Transform player2;

    [Tooltip("Vertical field of view used for the reference composition.")]
    public float verticalFov = 46f;
    [Tooltip("Camera tilt upwards (degrees) so the dome and chandelier stay in frame.")]
    public float pitchUp = 7.5f;
    [Tooltip("How quickly the camera catches up with the fighters.")]
    public float followSharpness = 6f;

    Vector3 panLeft, panRight, forward;
    Transform fightLeft, fightRight, p1Spawn, p2Spawn;
    bool ready;
    Camera cam;

    /// World positions of the stage walls on the fight line (use to clamp characters).
    public Vector3 FightLineLeft => fightLeft ? fightLeft.position : Vector3.zero;
    public Vector3 FightLineRight => fightRight ? fightRight.position : Vector3.zero;
    public Vector3 P1Spawn => p1Spawn ? p1Spawn.position : Vector3.zero;
    public Vector3 P2Spawn => p2Spawn ? p2Spawn.position : Vector3.zero;

    void OnEnable() { Setup(); }

    static Transform Find(Transform root, string name)
    {
        foreach (var t in root.GetComponentsInChildren<Transform>(true))
            if (t.name == name) return t;
        return null;
    }

    public void Setup()
    {
        cam = GetComponent<Camera>();
        ready = false;
        if (stageRoot == null) return;
        var l = Find(stageRoot, "MK_CameraPanLeft");
        var r = Find(stageRoot, "MK_CameraPanRight");
        fightLeft = Find(stageRoot, "MK_FightLine_Left");
        fightRight = Find(stageRoot, "MK_FightLine_Right");
        p1Spawn = Find(stageRoot, "MK_P1_Spawn");
        p2Spawn = Find(stageRoot, "MK_P2_Spawn");
        if (l == null || r == null || fightLeft == null || fightRight == null) return;
        panLeft = l.position; panRight = r.position;
        // Look horizontally from the camera rail towards the fight line - robust to any axis convention.
        Vector3 railMid = (panLeft + panRight) * 0.5f;
        Vector3 lineMid = (fightLeft.position + fightRight.position) * 0.5f;
        forward = Vector3.ProjectOnPlane(lineMid - railMid, Vector3.up).normalized;
        ready = forward.sqrMagnitude > 0.5f;
    }

    void LateUpdate()
    {
        if (!ready) { Setup(); if (!ready) return; }
        Vector3 axis = panRight - panLeft;
        float len = axis.magnitude; axis /= len;

        float target = 0.5f;
        if (player1 && player2) target = Proj((player1.position + player2.position) * 0.5f, axis, len);
        else if (player1) target = Proj(player1.position, axis, len);

        float t = target;
        if (Application.isPlaying)
        {
            float cur = Mathf.Clamp01(Vector3.Dot(transform.position - panLeft, axis) / len);
            t = Mathf.Lerp(cur, target, 1f - Mathf.Exp(-followSharpness * Time.deltaTime));
        }
        transform.position = Vector3.Lerp(panLeft, panRight, t);
        transform.rotation = Quaternion.LookRotation(forward, Vector3.up) * Quaternion.Euler(-pitchUp, 0f, 0f);
        cam.fieldOfView = verticalFov;
    }

    // Maps a world position to 0..1 along the pan rail. The fighters' range on the fight line
    // (between the MK_FightLine markers) is remapped so the camera reaches its stops as they reach the walls.
    float Proj(Vector3 p, Vector3 axis, float len)
    {
        float a = Vector3.Dot(fightLeft.position - panLeft, axis);
        float b = Vector3.Dot(fightRight.position - panLeft, axis);
        float x = Vector3.Dot(p - panLeft, axis);
        float camHalfView = 0.5f * (b - a) - 0.5f * len;   // keeps both walls framed at the stops
        return Mathf.Clamp01(Mathf.InverseLerp(a + camHalfView, b - camHalfView, x));
    }
}
