//Autor: Cesar Tadeu Pozzer

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     brilho;
uniform vec2 mouse;
uniform int pressedKey;
uniform float rotationAngle;
uniform float zoom;

varying float  x_coord;
varying float  y_coord;

void main()
{
    if (pressedKey == 48)
    {
        // og -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
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
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 49)
    {
    // circulo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
    float d = distance(vec2(x_coord, y_coord), vec2(0, 0));
    int circles = 1 + (mouse.y / 60);
    float step = 2.0 / circles;

    if (int(d / step) % 2 == 0)
    {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 50)
    {
        // interpola -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
        vec2 position = gl_TexCoord[0].st;
        vec3 cor;

        float t = (x_coord + 2.0) / 4.0;

        cor = texture2D(texture_0, position).rgb * t;
        cor += texture2D(texture_1, position).rgb * (1.0 - t);
        cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
        gl_FragColor = vec4( cor, 1.0);
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 51)
    {
        // Barras - A  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
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
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 52)
    {
        // Barras - B  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
        vec2 position = gl_TexCoord[0].st;
        vec3 cor;

        int steps = 12;
        float step = 600 / steps;

        bool invert = false;
    
        if (int(gl_FragCoord.x / step) % 2 == 0)
        {
            cor = texture2D(texture_0, position).rgb;
        }
        else
        {
            cor = texture2D(texture_1, position).rgb;
        }

        cor = clamp(cor, 0.0, 1.0);
   
        gl_FragColor = vec4( cor, 1.0);
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 53)
    {
        // grayscale  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
        vec2 position = gl_TexCoord[0].st;
        vec4 cor;
        cor = texture2D(texture_0, position).rgba; 
        cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
        float y = 0.3 * cor.r + 0.66 * cor.g + 0.2 * cor.b;
        cor = vec4(y, y, y, 1);

        //gl_FragColor = texture2D(texture, position).rgba;
        gl_FragColor = vec4(cor);
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 54)
    {
        // rotation -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        vec2 position = gl_TexCoord[0].st;
        vec3 cor;

        const float PI = 3.14159265358979323846;
    
        float theta = atan(y_coord, x_coord);
        if (theta < 0.0) theta += PI * 2;

        float angle = mod(rotationAngle, 2 * PI);
        if (angle < 0.0) angle += 2 * PI;

        float delta = mod(theta - angle + 2 * PI, 2 * PI);

        if(delta < PI)
           cor = texture2D(texture_0, position).rgb; //nao pega o alpha
        else
           cor = texture2D(texture_1, position).rgb; //nao pega o alpha
        cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros
   
        //gl_FragColor = texture2D(texture, position).rgba;
        gl_FragColor = vec4( cor, 1);
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }
    else if (pressedKey == 55)
    {
        // blur  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
        vec2 position = gl_TexCoord[0].st;
        vec4 cor = vec4(0.0);
        float kernelRadius = 0.05;
        int cont = 0;

        for (float i = 0; i < kernelRadius * 2; i += 0.001)
        {
            for (float j = 0; j < kernelRadius * 2; j += 0.001)
            {
                vec2 pos = vec2(position.x + i, position.y + j);
                cor += texture2D(texture_0, pos).rgba; 
                cont++;
            }
        }

        cor /= cont;
        cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros

        //gl_FragColor = texture2D(texture, position).rgba;
        gl_FragColor = vec4(cor);
    }
    else if (pressedKey == 56)
    {
        // lanterna
        vec2 position = gl_TexCoord[0].st;
        vec3 cor = texture2D(texture_0, position).rgb;
        cor = clamp(cor, 0.0, 1.0); 

        float r = 50.0;

        if (distance(vec2(gl_FragCoord.x, gl_FragCoord.y), mouse) > r)
        {
            cor = vec3(0, 0, 0);
        }
   
        //gl_FragColor = texture2D(texture, position).rgba;
        gl_FragColor = vec4( cor, 1);
    }
    else if (pressedKey == 57)
    {
        // lupa
        vec2 position = gl_TexCoord[0].st;
        vec3 cor = texture2D(texture_0, position).rgb;
        cor = clamp(cor, 0.0, 1.0); 

        float r = 0.2;

        vec2 mouseUV = vec2(mouse.x / 600.0, mouse.y / 600.0);
        float d = distance(position, mouseUV);

        if (d < r)
        {
            vec2 magnifiedUV = mouseUV + (position - mouseUV) / zoom;
            cor = texture2D(texture_0, magnifiedUV).rgb;
        }
        else
        {
            cor = texture2D(texture_0, position).rgb;
        }

        gl_FragColor = vec4( cor, 1);
    }
    else
    {
        // og -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-
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
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    }

}