// Additive glow sprites (chandelier bulbs, halo, floor light pool, sconces).
Shader "PS2Stage/Glow"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Color ("Tint", Color) = (1,1,1,1)
        _Intensity ("Intensity", Range(0, 4)) = 1.0
        _Flicker ("Candle flicker", Range(0, 0.5)) = 0.06
    }
    SubShader
    {
        Tags { "RenderType"="Transparent" "Queue"="Transparent" "IgnoreProjector"="True" }
        Pass
        {
            Blend One One
            ZWrite Off
            Cull Off
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            sampler2D _MainTex; float4 _MainTex_ST; fixed4 _Color; half _Intensity; half _Flicker;
            struct appdata { float4 vertex : POSITION; float2 uv : TEXCOORD0; fixed4 color : COLOR; };
            struct v2f { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; fixed4 color : COLOR; };
            v2f vert (appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                float3 w = mul(unity_ObjectToWorld, v.vertex).xyz;
                float f = 1.0 + _Flicker * sin(_Time.y * 9.0 + w.x * 3.1 + w.z * 1.7) * sin(_Time.y * 5.3 + w.y);
                fixed4 c = v.color;
                #ifndef UNITY_COLORSPACE_GAMMA
                c.rgb = GammaToLinearSpace(c.rgb);
                #endif
                o.color = c * _Color * (_Intensity * f);
                return o;
            }
            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 t = tex2D(_MainTex, i.uv);
                return fixed4(t.rgb * i.color.rgb, 1);
            }
            ENDCG
        }
    }
}
