#include "ObjLoader.h"


_ObjMesh::_ObjMesh(){
	loaded = false;
}
bool _ObjMesh::load(std::string file){
	double tstart, tstop;
	tstart = (double)clock()/CLOCKS_PER_SEC;

	free();

	std::cout<<"\n\nLoading: "<<file;

    _ObjStream buffer;
	
	if(!buffer.load(file.c_str())){
		std::cout<<"\nError opening "<<file.c_str();
		return false;
	}

	std::string auxString;
	std::vector<_ObjFace> face;
	
	_ObjMaterial *auxMaterial;
	m.push_back(_ObjMaterial());	//default material
	auxMaterial = &m[0];

	//Armazena os Vértices e as Faces
    while(buffer.good()){
		switch(buffer.getKeyword()){
            case OBJ_V:
				v.push_back( buffer.getPoint() );
                break;
			case OBJ_VN:
				n.push_back( buffer.getPoint() );
                break;
            case OBJ_VT:
				uv.push_back( buffer.getUV() );
				uv.back().v = 1 - uv.back().v;
                break;
            case OBJ_F:
				buffer.getFace(face);
				for(int i = 0; i < (int)face.size(); i++)
				{
					face[i].material = auxMaterial;
					f.push_back(face[i]);
				}
                break;
            case OBJ_USEMTL:
				if(!m.empty()){
					auxString.clear();
					buffer.getName(auxString);
					for(int i = 0 ; i < (int)m.size() ; i++){
						if(!auxString.compare(m[i].name)){
							auxMaterial = &m[i];
							break;
						}
					}
				}
                break;
			case OBJ_MTLLIB:
				//Separa o caminho do nome do arquivo
				std::string path;
				std::size_t pos = file.rfind('\\');
				path = file.substr(0, pos+1);
				auxString.clear();
				buffer.getName(auxString);
				loadMaterial(path, auxString);
				break;
		}
    }
    buffer.free();
	
	/*std::cout<<"\nVertices:           "<<v.size();
	std::cout<<"\nNormals:            "<<n.size();
	std::cout<<"\nUVs:                "<<uv.size();
	std::cout<<"\nTriangles:          "<<f.size();*/


	if(f.size() == 0 || v.size() == 0){
		loaded = false;
		std::cout<<"\nNo face or vertex has been found!";
		return false;
	}
	tstop = (double)clock()/CLOCKS_PER_SEC;
	std::cout<<"\nLoading time:       "<<tstop-tstart<<"s";
	std::cout<<"\nDone!\n\n";
	loaded = true;
	return true;
}
bool _ObjMesh::loadMaterial(std::string path, std::string name){
	std::cout<<"\nLoading: "<<path<<name;

	std::string auxString;
	auxString = path + name;

	_ObjStream buffer;
	
	if(!buffer.load(auxString.c_str())){
		std::cout<<"\nError opening "<<auxString.c_str();
		return false;
	}
	
	_ObjMaterial material;
	//Armazena os Vértices e as Faces
	while(buffer.good()){
		switch(buffer.getKeyword()){
            case MTL_NEWMTL:
				auxString.clear();
                buffer.getName(auxString);
				material.name = auxString;
				m.push_back(material);
                break;
			case MTL_KA:
				buffer.getRGB(m.back().ambient);
				break;
			case MTL_KD:
				buffer.getRGB(m.back().diffuse);
				break;
			case MTL_KS:
				buffer.getRGB(m.back().specular);
				break;
			case MTL_MAP_KD:
				std::string imagePath;
				buffer.getName(imagePath);
				if(imagePath.find(":\\") != -1 || imagePath[0] == '/'){
					loadTexture(imagePath);
				}else{
					loadTexture(path + imagePath);
				}
				break;
		}
	}

	/*for(int i = 0; i < m.size(); i++){
		std::cout<<"\nMaterial "<<i;
		std::cout<<"\n  Name: "<<m[i].name;
		std::cout<<"\n  Ambient: "<<m[i].ambient.r<<"r  "<<m[i].ambient.g<<"g  "<<m[i].ambient.b<<"b";
		std::cout<<\n  Diffuse: "<<m[i].diffuse.r<<"r  "<<m[i].diffuse.g<<"f  "<<m[i].diffuse.b<<"b";
		std::cout<<\n  Specular: "<<m[i].specular.r<<"r  "<<m[i].specular.g<<"g  "<<m[i].specular.b<<"b  \n";
	}*/
	
	buffer.free();

	return true;
}
bool _ObjMesh::loadTexture(std::string path){
	std::cout<<"\nLoading: "<<path;
	
	int width, height, comp;
	GLubyte *data;

	FILE *file;
	if(!(file = fopen(path.c_str(), "rb"))){
		std::cout<<"\nError opening "<<path.c_str();
		return false;
	}
	data = stbi_load_from_file(file, &width, &height, &comp, 0);
	if(!data){
		std::cout<<"\nError reading image!";
		return false;
	}
	fclose(file);

	/*std::cout<<"\nWidth:  "<<width;
	std::cout<<"\nHeight: "<<height;
	std::cout<<"\nComp:   "<<comp;*/

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m.back().texture);
	glBindTexture(GL_TEXTURE_2D, m.back().texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(comp == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}else if(comp == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);

	m.back().textured = true;
	return true;
}
void _ObjMesh::free(void){
	v.clear();
	uv.clear();
	n.clear();
	m.clear();
	f.clear();
	loaded = false;
}




_ObjMaterial::_ObjMaterial(){
	_ObjMaterial::name = "___default";
	_ObjMaterial::ambient.r = 0.6f;
	_ObjMaterial::ambient.g = 0.6f;
	_ObjMaterial::ambient.b = 0.6f;
	_ObjMaterial::diffuse.r = 0.6f;
	_ObjMaterial::diffuse.g = 0.6f;
	_ObjMaterial::diffuse.b = 0.6f;
	_ObjMaterial::specular.r = 0.8f;
	_ObjMaterial::specular.g = 0.8f;
	_ObjMaterial::specular.b = 0.8f;
	_ObjMaterial::texture = 0;
	_ObjMaterial::textured = false;
}


_ObjFace::_ObjFace(){
	material = NULL;
}




_ObjStream::_ObjStream(){
    iterator = 0;
    length = 0;
    buffer = NULL;
}

_ObjStream::_ObjStream(const char *path){
    load(path);
}
bool _ObjStream::load(const char *path){
    //load file
    std::ifstream file(path, std::ifstream::in);
	if(!file.is_open()){
		return false;
	}
    //get the length of the file
    file.seekg (0, std::ios::end);
    length = (int)file.tellg();
    file.seekg (0, std::ios::beg);
    //allocate
    buffer = new char [length];
    //read the entire file to buffer
    file.read (buffer,length);
    //close file
	file.close();
	iterator = 0;
	return true;
}
void _ObjStream::free(void){
    delete [] buffer;
    buffer = NULL;
    iterator = 0;
    length = 0;
}
bool _ObjStream::good(void){
    if(iterator < length){
        return true;
    }
    return false;
}
int  _ObjStream::getKeyword(void){
    while(good()){
		ignoreTillNext();
        switch(buffer[iterator]){
			case 'o':
				iterator++;
				return OBJ_GROUP;
				break;
			case 'g':
				iterator++;
				return OBJ_GROUP;
				break;
            case 'v':
                iterator++;
                switch(buffer[iterator]){
                    case ' ':
                        return OBJ_V;
                        break;
                    case 't':
                        iterator++;
                        return OBJ_VT;
                        break;
                    case 'n':
                        iterator++;
                        return OBJ_VN;
                        break;
                }
                break;
            case 'f':
                iterator++;
                return OBJ_F;
                break;
            case 'm':
                iterator++;
                if((buffer[iterator] == 't') && (buffer[iterator+1] == 'l') && (buffer[iterator+2] == 'l') && (buffer[iterator+3] == 'i') && (buffer[iterator+4] == 'b')){
                    iterator += 5;
                    return OBJ_MTLLIB;
                }
				if((buffer[iterator] == 'a') && (buffer[iterator+1] == 'p') && (buffer[iterator+2] == '_') && (buffer[iterator+3] == 'K') && (buffer[iterator+4] == 'd')){
					iterator += 5;
					return MTL_MAP_KD;
				}
                break;
            case 'u':
                iterator++;
                if((buffer[iterator] == 's') && (buffer[iterator+1] == 'e') && (buffer[iterator+2] == 'm') && (buffer[iterator+3] == 't') && (buffer[iterator+4] == 'l')){
                    iterator += 5;
                    return OBJ_USEMTL;
                }
                break;
			case 'n':
                iterator++;
                if((buffer[iterator] == 'e') && (buffer[iterator+1] == 'w') && (buffer[iterator+2] == 'm') && (buffer[iterator+3] == 't') && (buffer[iterator+4] == 'l')){
                    iterator += 5;
                    return MTL_NEWMTL;
                }
                break;
			case 'K':
                iterator++;
                switch(buffer[iterator]){
                    case 'a':
                        return MTL_KA;
                        break;
                    case 'd':
                        iterator++;
                        return MTL_KD;
                        break;
                    case 's':
                        iterator++;
                        return MTL_KS;
                        break;
                }
                break;
        }
		//ignore the entire line
        for(iterator; iterator < length; iterator++){
            if(buffer[iterator]=='\n'){
                iterator++;
                break;
            }
        }

    }
    return -1;
}
void _ObjStream::ignore(int n, char c){
    if((n<0) || (iterator + n >= length)){
        n = length;
    }else{
        n += iterator;
    }
    for(iterator ; (iterator < n)&&(buffer[iterator]!=c) ; iterator++){}
    if(iterator < length-1){
        iterator++;
    }
}
void _ObjStream::ignore(char c){
    ignore(-1, c);
}
void _ObjStream::ignoreTillNext(void){
	while(good()){
		if(buffer[iterator] == ' ' || buffer[iterator] == '\t' || buffer[iterator] == '\n'){
			iterator++;
		}else{
			return;
		}
	}
}
void _ObjStream::begin(void){
	iterator = 0;
}
bool _ObjStream::checkNumerical(){
	if((buffer[iterator] >= '0' && buffer[iterator] <= '9')){
		return true;	
	}
	if(buffer[iterator] == '-' || buffer[iterator] == '+' || buffer[iterator] == '.'){
		return true;
	}
	return false;
}
bool _ObjStream::checkNumerical(char c){
	if((c > '0' && c < '9') || c == '-' || c == '+' || c == '.'){
		return true;
	}
	return false;
}
struct _ObjPoint3f _ObjStream::getPoint(){
	
	struct _ObjPoint3f v;
	char aux[50];
	int i;

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && i<50; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	v.x = (float)atof(aux);

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && i<50; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	v.y = (float)atof(aux);

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && i<50; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	v.z = (float)atof(aux);

	ignore('\n');

	return v;
}
struct _ObjUV _ObjStream::getUV(){

	_ObjUV uv;
	char aux[50];
	int i;

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && i<50; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	uv.u = (float)atof(aux);

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && i<50; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	uv.v = (float)atof(aux);

	ignore('\n');

	return uv;
}
void _ObjStream::getFace(std::vector<_ObjFace> &face){
	
	face.clear();

	class _ObjFace f;
	char aux[40];
	int i;

	for(int fi = 0 ; fi < 3 ; fi++){

		ignoreTillNext();
		//V
		for(i = 0, iterator; checkNumerical() && good() && (i<40) ; i++, iterator++){
			aux[i] = buffer[iterator];
		}
		aux[i] = '\0';
		f.v[fi] = atoi(aux);
		f.v[fi]--;
		if(buffer[iterator] == '/'){
			if(buffer[iterator + 1] == '/'){
				iterator++;
				//VN
				for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
					aux[i] = buffer[iterator];
				}
				aux[i] = '\0';
				f.n[fi] = atoi(aux);
				f.n[fi]--;
			}else{
				//VT
				for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
					aux[i] = buffer[iterator];
				}
				aux[i] = '\0';
				f.uv[fi] = atoi(aux);
				f.uv[fi]--;
				if(buffer[iterator] == '/'){
					//VN
					for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
						aux[i] = buffer[iterator];
					}
					aux[i] = '\0';
					f.n[fi] = atoi(aux);
					f.n[fi]--;
				}
			}
		}
	}

	ignoreTillNext();

	if(buffer[iterator] >= '0' && buffer[iterator] <= '9')		//quad, need to be triangulated
	{
		_ObjFace f2;
		f2.n[0] = f.n[2]; f2.v[0] = f.v[2]; f2.uv[0] = f.uv[2];

		//V
		for(i = 0, iterator; checkNumerical() && good() && (i<40) ; i++, iterator++){
			aux[i] = buffer[iterator];
		}
		aux[i] = '\0';
		f2.v[1] = atoi(aux);
		f2.v[1]--;
		if(buffer[iterator] == '/'){
			if(buffer[iterator + 1] == '/'){
				iterator++;
				//VN
				for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
					aux[i] = buffer[iterator];
				}
				aux[i] = '\0';
				f2.n[1] = atoi(aux);
				f2.n[1]--;
			}else{
				//VT
				for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
					aux[i] = buffer[iterator];
				}
				aux[i] = '\0';
				f2.uv[1] = atoi(aux);
				f2.uv[1]--;
				if(buffer[iterator] == '/'){
					//VN
					for(i = 0, iterator++ ; checkNumerical() && good() && (i<40) ; i++, iterator++){
						aux[i] = buffer[iterator];
					}
					aux[i] = '\0';
					f2.n[1] = atoi(aux);
					f2.n[1]--;
				}
			}
		}

		f2.n[2] = f.n[0]; f2.v[2] = f.v[0]; f2.uv[2] = f.uv[0];

		face.push_back(f);
		face.push_back(f2);
	}
	else														//triangle
	{
		face.push_back(f);
	}

}

void _ObjStream::getName(std::string &name){
	char aux[100];
	int i;

	ignoreTillNext();
	for(i = 0, iterator; (buffer[iterator] != '\n') && good() && (i<100); i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	name = aux;

	ignore('\n');
}
void _ObjStream::getRGB(struct _ObjRGB &color){

	char aux[40];
	int i;

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && (i<40) ; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	color.r = (float)atof(aux);

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && (i<40) ; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	color.g = (float)atof(aux);

	ignoreTillNext();
	for(i = 0, iterator; checkNumerical() && good() && (i<40) ; i++, iterator++){
		aux[i] = buffer[iterator];
	}
	aux[i] = '\0';
	color.b = (float)atof(aux);

	ignore('\n');
}