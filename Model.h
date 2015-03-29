#include <vector>

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskanik na obiekt reprezentujšcy program cieniujšcy.

void assignVBOtoAttribute(char* attributeName, GLuint bufVBO, int variableSize) {
	GLuint location=shaderProgram->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //Włšcz używanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location,variableSize,GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location majš być brane z aktywnego VBO
}
GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;
	
	glGenBuffers(1,&handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który będzie zawierał tablicę danych
	glBindBuffer(GL_ARRAY_BUFFER,handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicę do VBO
	
	return handle;
}


class Model{
public:
GLuint vao;
GLuint bufVertices; //Uchwyt na bufor VBO przechowujšcy tablicę wsp. wierzch
//GLuint bufColors;  //Uchwyt na bufor VBO przechowujšcy tablicę kolorów
GLuint bufNormals; //Uchwyt na bufor VBO przechowujšcy tablicę wektorów norm.
GLuint bufTexCoords;
GLuint tex0;
GLuint tex1;
float *vertices;
float *normals;
float *texCoords;
int vertexCount;
//float *colors=teapotColors;

void draw(){

	glUniform1i(shaderProgram->getUniformLocation("textureMap0"),0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"),1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,this->tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,this->tex1);


	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powišzań slotów atrybutów z tablicami z danymi
	glBindVertexArray(this->vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES,0,this->vertexCount);
	
	//Posprzštanie po sobie (niekonieczne w sumie jeżeli korzystamy z VAO dla każdego rysowanego obiektu)
	glBindVertexArray(0);
}
void drawtex(GLuint tex2){

	glUniform1i(shaderProgram->getUniformLocation("textureMap0"),0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"),1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,this->tex1);


	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powišzań slotów atrybutów z tablicami z danymi
	glBindVertexArray(this->vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES,0,this->vertexCount);
	
	//Posprzštanie po sobie (niekonieczne w sumie jeżeli korzystamy z VAO dla każdego rysowanego obiektu)
	glBindVertexArray(0);
}
void setupVBO() {
	this->bufVertices=makeBuffer(this->vertices, this->vertexCount, sizeof(float)*4); //Współrzędne wierzchołków
//	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);//Kolory wierzchołków
	this->bufNormals=makeBuffer(this->normals, this->vertexCount, sizeof(float)*4);//Wektory normalne wierzchołków
	this->bufTexCoords=makeBuffer(this->texCoords, vertexCount, sizeof(float)*2);//Wektory normalne wierzchołków
}
void setupVAO() {
	//Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	glGenVertexArrays(1,&(this->vao));
	
	//Uaktywnij nowo utworzony VAO
	glBindVertexArray(this->vao);

	assignVBOtoAttribute("vertex",this->bufVertices,4); //"vertex" odnosi się do deklaracji "in vec4 vertex;" w vertex shaderze
//	assignVBOtoAttribute("color",bufColors,4); //"color" odnosi się do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute("normal",this->bufNormals,4); //"normal" odnosi się do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute("texCoord",bufTexCoords,2); //"texCoord" odnosi się do deklaracji "in vec2 texCoord;" w vertex shaderze
	glBindVertexArray(0);
}

void freeVBO() {
	glDeleteBuffers(1,&this->bufVertices);
//	glDeleteBuffers(1,&this->bufColors);
	glDeleteBuffers(1,&this->bufNormals);
}

void freeVAO() {
	glDeleteVertexArrays(1,&this->vao);
}

};

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}

	return true;
}
