

#version 150


in vec4 vertex_color;
in vec4 vertex_pos;
flat in int vertex_id_g;
flat in vec4 vertex_start;
flat in vec4 vertex_end;

out vec4 Out_Color;
uniform int dotted;

void main()
{

	if(dotted!=0){
		float cumDist = distance(vertex_start.xy, vertex_end.xy) * vertex_id_g;

		if (cos(0.4*
				(abs(cumDist) + abs(distance(vertex_start.xy, vertex_pos.xy)))
				) - 0.5 > 0.0) {
			Out_Color = vec4(0,0,0,0);
			return;
		}
	}
	Out_Color = vertex_color;

}
