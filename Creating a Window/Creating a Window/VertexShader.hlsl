struct VOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//Vertex shader
VOut main( float4 pos : POSITION, float4 color : COLOR )
{
	//Create VOut struct
	VOut output;

	output.position = pos;
	output.color = color;


	return output;
}