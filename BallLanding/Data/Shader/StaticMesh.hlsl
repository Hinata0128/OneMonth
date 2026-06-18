/*********************************************************************
*	スタティックメッシュクラス用シェーダファイル.
**/
//グローバル変数.
//テクスチャは レジスタ t(n).
Texture2D g_Texture : register(t0);
//サンプラは レジスタ s(n).
SamplerState g_SamLinear : register(s0);

//コンスタントバッファ.
//メッシュ単位.
cbuffer per_mesh : register(b0)
{
    matrix g_mW; //ワールド行列.
    matrix g_mWVP; //ワールド,ビュー,プロジェクションの合成行列.
};

//マテリアル単位.
cbuffer per_material : register(b1)
{
    float4 g_Diffuse; //ディフューズ色(拡散反射色).
    float4 g_Ambient; //アンビエント色(環境色).
    float4 g_Specular; //スペキュラ色(鏡面反射色).
    float g_IsLight; //【追加】ライト有効フラグ (1.0で有効、0.0で無効).
    float3 g_Padding; //【追加】16バイト境界に合わせるための詰め物.
};

//フレーム単位.
cbuffer per_frame : register(b2)
{
    float4 g_CameraPos; //カメラ位置(視点位置).
    float4 g_vLightDir; //ライトの方向ベクトル.
};

//頂点シェーダの出力パラメータ.
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 Normal : TEXCOORD0;
    float2 UV : TEXCOORD1;
    float3 Light : TEXCOORD2;
    float3 EyeVector : TEXCOORD3;
    float4 PosWorld : TEXCOORD4;
    float4 Color : COLOR;
};

//-------------------------------------------------
//	頂点(バーテックス)シェーダ.
//-------------------------------------------------
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float4 Norm : NORMAL,
	float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
    output.Light = normalize(g_vLightDir.xyz);

    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
    output.EyeVector = normalize(g_CameraPos.xyz - output.PosWorld.xyz);

	//テクスチャ座標.
    output.UV = UV;

    return output;
}

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//テクスチャカラー.
    float4 texColor = g_Texture.Sample(g_SamLinear, input.UV);

	//【追加】ライト無効設定（スカイボックス等）の判定
	// プログラム側で g_IsLight を 0.0f に設定した場合、
	// ライト計算をスキップしてテクスチャの色をそのまま返す.
    if (g_IsLight < 0.5f)
    {
        return texColor;
    }

	//環境光　※１.
    float4 ambient = texColor * g_Ambient;

	//拡散反射光 ※２.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = (g_Diffuse / 2 + texColor / 2) * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + diffuse + specular;
    return Color;
}

//========= テクスチャ無し用 ========================================
//-------------------------------------------------
//	頂点(バーテックス)シェーダ.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(
	float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
    output.Light = normalize(g_vLightDir.xyz);

    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
    output.EyeVector = normalize(g_CameraPos.xyz - output.PosWorld.xyz);

    return output;
}

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//【追加】ライト無効設定の判定
    if (g_IsLight < 0.5f)
    {
        return g_Diffuse; // テクスチャなしの場合はディフューズ色を返す
    }

	//環境光　※１.
    float4 ambient = g_Ambient;

	//拡散反射光 ※２.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = g_Diffuse * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + diffuse + specular;
    return Color;
}