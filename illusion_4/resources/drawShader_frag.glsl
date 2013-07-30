
uniform sampler2D vid;

void main() {
    gl_FragData[0] = texture2D(vid, vec2(gl_TexCoord[0].s, 1.- gl_TexCoord[0].t));
}
