
//uniform mat4 mv, proj;
uniform sampler2D tex;
varying vec4 cur;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    cur = texture2D(tex, gl_TexCoord[0].st);
    vec2 pos = cur.xy;
  //  pos.x *= 1280.;
  //  pos.y *= 800.;
   	gl_Position =  gl_ModelViewProjectionMatrix * vec4(pos, 0., 1.);
}
