#include "MeshRendererSystem.h"

#include <stb_image.h>
#include <entt/entity/registry.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Texture.h"
#include "../Components/PointLight.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		const std::string error = "Texture failed to load at path: " + std::string(path);
		stbi_image_free(data);
		throw std::exception(error.c_str());
	}

	return textureID;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<TextureComponent> MeshRendererSystem::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureComponent> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				// a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			TextureComponent texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
			// store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;
}

void MeshRendererSystem::ProcessMesh(
	aiMesh* mesh,
	const aiScene* scene,
	std::vector<Vertex> &vertices,
	std::vector<unsigned int> &indices,
	std::vector<TextureComponent> &textures
)
{
	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoordinates = vec;
		}
		else
			vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	
	// 1. diffuse maps
	std::vector<TextureComponent> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<TextureComponent> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
	// TODO: Other maps
}

void MeshRendererSystem::Setup(entt::registry &registry)
{
    auto view = registry.view<MeshRenderer>();
    view.each([this](MeshRenderer &meshRenderer)
    {
        // Load model
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(meshRenderer.modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            const std::string error = "Error loading model: " + std::string(importer.GetErrorString());
            throw std::exception(error.c_str());
        }
    	// TODO: Don't load textures this way
    	directory = meshRenderer.modelPath.substr(0, meshRenderer.modelPath.find_last_of("/"));
    	// TODO: Logic for loading in sub-meshes
    	aiMesh* mesh = scene->mMeshes[0];
        ProcessMesh(mesh, scene, meshRenderer.vertices, meshRenderer.indices, meshRenderer.textures);

        // Setup mesh
        glGenVertexArrays(1, &meshRenderer.vao);
        glGenBuffers(1, &meshRenderer.vbo);
        glGenBuffers(1, &meshRenderer.ebo);
      
        glBindVertexArray(meshRenderer.vao);
        glBindBuffer(GL_ARRAY_BUFFER, meshRenderer.vbo);

        glBufferData(GL_ARRAY_BUFFER, meshRenderer.vertices.size() * sizeof(Vertex), &meshRenderer.vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRenderer.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshRenderer.indices.size() * sizeof(unsigned int), 
                     &meshRenderer.indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

        glBindVertexArray(0);
    });
    
    glEnable(GL_DEPTH_TEST);
}

void MeshRendererSystem::Update(entt::registry &registry)
{
    auto view = registry.view<MeshRenderer, Transform>();
    
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = scene->camera;
    auto window = scene->window;
    auto lightDirection = scene->light->lightDirection;
    auto lightColor = scene->light->lightColor;
    
    auto frontEntity = view.front();
    auto frontRenderer = registry.get<MeshRenderer>(frontEntity);
    auto shader = frontRenderer.shader;
    
    shader->Use();
    shader->SetVec3("material.specular",glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetFloat("material.shininess", 64.0f);
    auto ambient = lightColor * 0.2f;
    auto diffuse = lightColor * 0.5f;
    auto specular = lightColor;

    // light values
    shader->SetVec3("directionalLight.direction", lightDirection);
    shader->SetVec3("directionalLight.ambient",  ambient);
    shader->SetVec3("directionalLight.diffuse",  diffuse);
    shader->SetVec3("directionalLight.specular", specular);

    // camera
    shader->SetVec3("viewPosition", camera->Position);
        
    // view and projection matrices
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), window->GetAspectRatio(), 0.1f, 100.0f);
    auto viewMatrix = camera->GetViewMatrix();
    shader->SetMat4("view", viewMatrix);
    shader->SetMat4("projection", projectionMatrix);

    view.each([shader](MeshRenderer &renderer, Transform &transform)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        glm::quat rotation;
        rotation = glm::quat(transform.rotation);
        model *= glm::toMat4(rotation);
        model = glm::scale(model, transform.scale);
        shader->SetMat4("model", model);
        //renderer.model->Draw(shader);
    	unsigned int diffuseIndex = 1;
		unsigned int specularIndex = 1;
		for(unsigned int i = 0; i < renderer.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = renderer.textures[i].type;
			if(name == "diffuse")
				number = std::to_string(diffuseIndex++);
			else if(name == "specular")
				number = std::to_string(specularIndex++);

			shader->SetInt(("material" + number + "." + name).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, renderer.textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(renderer.vao);
		glDrawElements(GL_TRIANGLES, renderer.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
    });
    
    /*auto lightView = registry.view<PointLight, Transform>();
    auto lightIndex = 0;
    for(auto entity: lightView) {
        auto &pointLight = lightView.get<PointLight>(entity);
        auto &transform = lightView.get<Transform>(entity);
        std::string pointLightUniform = "pointLights[" + std::to_string(lightIndex) + "]";
        shader->SetVec3(pointLightUniform + ".position", transform.position);
        shader->SetVec3(pointLightUniform + ".ambient", pointLight.ambient);
        shader->SetVec3(pointLightUniform + ".diffuse", pointLight.diffuse);
        shader->SetVec3(pointLightUniform + ".specular", pointLight.specular);
        shader->SetFloat(pointLightUniform + ".constant", pointLight.constant);
        shader->SetFloat(pointLightUniform + ".linear", pointLight.linear);
        shader->SetFloat(pointLightUniform + ".quadratic", pointLight.quadratic);
        lightIndex++;
    }*/
}
