#include "Model.h"


Model::Model()
{
}

void Model::renderModel()
{
	for (size_t i = 0; i < mesh_list.size(); i++)
	{
		unsigned int materialIndex = mesh_to_texture[i];

		if (materialIndex < texture_list.size() && texture_list[materialIndex])
		{
			texture_list[materialIndex]->use_texture();
		}

		mesh_list[i]->render_mesh();
	}
}

void Model::loadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		printf("Failed to load the scene %s!\nError string: %s", fileName, importer.GetErrorString());
	}

	load_node(scene->mRootNode, scene);
	load_materials(scene);
}

void Model::load_node(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		load_mesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		load_node(node->mChildren[i], scene);
	}
}

void Model::load_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->create_mesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	mesh_list.push_back(newMesh);
	mesh_to_texture.push_back(mesh->mMaterialIndex);
}

void Model::load_materials(const aiScene* scene)
{
	texture_list.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		texture_list[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string texturePath = std::string("Textures/") + filename;

				texture_list[i] = new Texture(texturePath.c_str());

				if (!texture_list[i]->load_texture_jpg())
				{
					printf("Failed to load Texture at path (for jpg): %s\n", texturePath);
					delete texture_list[i];
					texture_list[i] = nullptr;
				}

				/*else if (!texture_list[i]->load_texture_jpg())
				{
					printf("Failed to load Texture at path (for png): %s\n", texturePath);
					delete texture_list[i];
					texture_list[i] = nullptr;
				}*/
			}
		}

		if (!texture_list[i])
		{
			texture_list[i] = new Texture("Textures/g1.png");
			texture_list[i]->load_texture_png();
		}
	}
}

void Model::clearModel()
{
	for (size_t i = 0; i < mesh_list.size(); i++)
	{
		if (mesh_list[i])
		{
			delete mesh_list[i];
			mesh_list[i] = nullptr;
		}
	}

	for (size_t i = 0; i < texture_list.size(); i++)
	{
		if (texture_list[i])
		{
			delete texture_list[i];
			texture_list[i] = nullptr;
		}
	}
}

Model::~Model()
{
}