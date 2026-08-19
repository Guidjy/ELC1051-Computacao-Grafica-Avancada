
//Autor: Cesar Tadeu Pozzer
// 2012
//aplica duas formas de iluminacao
// - no espaco do olho
// - no espaco do objeto


#version 120

uniform vec3 lightSourcePos;
uniform int  eye_world;

varying vec3 luzPos;
varying vec3 normal;
varying vec3 coord;


void main()
{

//****************************************************************
// * Aplica a iluninacao no espaco do mundo. Nao é aplicado nenhuma transformacao sobre coordeandas do quadrado, normais e posicao da fonte luminosa.
// * A coordenada gl_Position é transformada para que o quadrado seja visualizado na tela.
// * A variavel coord contem as coordenadas originais do quadrado, e é usada no calculo de iluminacao
//****************************************************************
    if( eye_world == 1)
    {
        //========================================================
        //lightSourcePos = gl_LightSource[0].position.xyz;
        //lightSourcePos nao pode ser normalizada aqui, somente apos a subtracao com a coordenada do pixel sendo renderizado
        //========================================================
        luzPos = lightSourcePos;
        //luzPos = gl_LightSource[0].position.xyz; //ja pode vir transformado para eye coordinates

        normal = normalize(gl_Normal);

        coord = vec3(gl_Vertex); //ou gl_Vertex.xyz

        gl_Position = ftransform();
    }

//****************************************************************
// * Aplica a iluninacao no espaco do olho (glutLookAt)
// * O vetor normal, coordenadas da fonte luminosa e coordendas do objeto devem ser transformadas para o espaco do olho.
//****************************************************************
    else if( eye_world == 0)
    {
        luzPos = vec3(gl_ModelViewMatrix * vec4(lightSourcePos,1));
        luzPos = gl_LightSource[0].position.xyz;

        normal = normalize(gl_NormalMatrix * gl_Normal);
        //normal = normalize( mat3(gl_ModelViewMatrix) * gl_Normal); //somente funciona se a modelview for ortogonal

        coord = vec3(gl_ModelViewMatrix * gl_Vertex);

        gl_Position = ftransform();
    }
}

