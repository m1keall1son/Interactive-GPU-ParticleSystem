
uniform sampler2D tex;
uniform sampler2D startTex;
uniform sampler2D displacement;
uniform float threshold;

const float maxVelocity = .2;

vec2 rand(vec2 A,vec2 B){
    
	vec2 s=vec2(12.9898,78.233);
	vec2 tmp=vec2(dot(A,s),dot(B,s));
    
	return fract(cos(tmp) * 43758.5453) * 2.0 - 1.0;
}


void main() {
	vec4 cur = texture2D(tex, gl_TexCoord[0].xy);
    vec4 start = texture2D(startTex, gl_TexCoord[0].xy);
    vec2 randomValx = abs(rand(cur.xw,cur.yz));
    vec2 randomValy = abs(rand(cur.xy,start.wz));

    if(cur.y < -1. || cur.y > 2.) {
        cur = start;
    }
    if(cur.x < -1. || cur.x > 2. ){
        cur = start;
    }

	vec2 curPos = cur.xy;
    vec4 displace = texture2D(displacement, vec2(gl_TexCoord[0].s, 1.-gl_TexCoord[0].t));
    vec2 d;
    d.x = displace.x;
    d.y = displace.y;
    vec2 curVel = (cur.zw - .5) * maxVelocity;
    vec2 nextVel = vec2(0.,0.);
    if(displace.r>threshold){
     //   nextVel = (randomValx/randomValy) * vec2(-.06,0.);
      nextVel =  (randomValx/(randomValy+10.)) * vec2(.001,.6);

    }else{
        curPos = start.xy;
        nextVel = vec2(0.,0.);
    }
    nextVel = mix(nextVel, curVel, .7) ;
	vec2 nextPos = (curPos + -nextVel);
    if(nextVel.x < 0. && nextVel.y < 0. && curVel.x < 0. && curVel.y < 0.){
        nextPos = start.xy;
    }
	gl_FragColor = vec4(nextPos.x, nextPos.y, nextVel / maxVelocity + .5);
  }
