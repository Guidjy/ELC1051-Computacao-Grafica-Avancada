//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     brilho;
varying float  x_coord;
varying float  y_coord;

void main()
{
    /* og -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
    vec2 position = gl_TexCoord[0].st;
    vec3 cor;
    if(x_coord < 0)
       cor = texture2D(texture_0, position).rgb; //nao pega o alpha
    else
       cor = texture2D(texture_1, position).rgb; //nao pega o alpha
    cor*=brilho;
    cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
    //gl_FragColor = texture2D(texture, position).rgba;
    gl_FragColor = vec4( cor, 1);
    -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    */

    /* circulo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
    float d = distance(vec2(x_coord, y_coord), vec2(0, 0));
    int circles = 15;
    float step = 2.0 / circles;

    if (int(d / step) % 2 == 0)
    {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    */ 

    /* interpola -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
    vec2 position = gl_TexCoord[0].st;
    vec3 cor;

    float t = (x_coord + 2.0) / 4.0;

    cor = texture2D(texture_0, position).rgb * t;
    cor += texture2D(texture_1, position).rgb * (1.0 - t);
    cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
    gl_FragColor = vec4( cor, 1.0);
    -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    */ 

    vec2 position = gl_TexCoord[0].st;
    vec3 cor;

    int steps = 6;
    float step = 2.0 / steps;

    bool invert = false;
    if (x_coord > 0.0) invert = true;
    
    if (int(x_coord / step) % 2 == 0)
    {
        if (invert)
        {
            cor = texture2D(texture_0, position).rgb;
        }
        else
        {
            cor = texture2D(texture_1, position).rgb;
        }
    }
    else
    {
        if (invert)
        {
            cor = texture2D(texture_1, position).rgb;
        }
        else
        {
            cor = texture2D(texture_0, position).rgb;
        }
    }

    cor = clamp(cor, 0.0, 1.0);
   
    gl_FragColor = vec4( cor, 1.0);

}