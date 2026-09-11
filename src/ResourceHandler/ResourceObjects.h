#pragma once

//Local
#include <GDHandler/ResourcePaths.h>
#include <ResourceHandler/ResourceReader.h>


// Third-party Headers
#include "yaml-cpp/yaml.h"

#include <monster/VulkanUtils.h>

// STL Headers
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>


// The Path storage in vectors are as follows
enum ResourcePaths {
	MasterList,
	Cases,
	Scenes,
	ScnObj,
	Objects,
	Shaders,
	ShaderVars,
	ShaderValues,
	MaterialPath,
	MaterialTexture,
	RenderMeshPath,
	ImagePath
};

namespace ResourceGroup {
	const std::vector < std::string > ResourceMasterGroups = {
	"Global",
		"Objects",
		"Shaders",
		"MaterialPath",
		"MaterialTexture",
		"RenderMeshPath",
		"ImagePath",
		"Font"
	};

	enum RESOURCE_MASTER_GROUP_INDEX { // MUST BE IN THE SAME ORDER AS ResourceMasterGroups
		GLOBAL,
		OBJECT,
		SHADER,
		MATERIAL_PATH,
		MATERIAL_TEXTURE,
		RENDER_MESH_PATH,
		IMAGE_PATH,
		FONT
	};
}

struct ResourceMasterGroup {

	ResourceMasterGroup(std::string name_p) {
		GroupName = name_p;
	}

	std::string GroupName;
	std::string loadPath; // name of the loadPath i.e. in ResourceLoader with containes the paths for all the contents fetched in this perticular Group
	int loadPathIndex;
	std::vector<std::filesystem::path>* ResourcePaths = new std::vector<std::filesystem::path>();

	// make load Paths as vector<> and imaplement String to path convertion in the ResourceLoader
};

typedef unsigned long long ResID;

enum PhysXType {
	Static,
	Dynamic,
	Kinematic
};


enum SceneType {
	STATIC,
	DYNAMIC,
	MESH
};

class ResourceHandlerIDError : public std::exception {
private:
	const char* cause;
public:
	ResourceHandlerIDError(const char* cause_p) {
		cause = cause_p;
	};
	char* what() {
		std::cout << std::endl << cause << std::endl;
	}

};

// Load Paths
// These paths are fetched when the application starts.
struct ResourceLoadPath {
	std::string pathGroupName;
	std::vector<std::string>* extensions;
	std::vector<std::string>* paths;

	std::string masterGroupName = "";
};

class CaseResource;
class SceneResource;
class ObjectResource;
class ShaderResource;
class MaterialResource;
class RenderMeshResource;
class ColliderMeshResource;
class ImageResource;


bool vectorPathContains(std::vector<std::filesystem::path>* checkVec, std::string file);

// MASTER RESOURCE CLASS. HANDLES STORAGE OF RESOURCE IDS and RESOURCES
class ResourceHandlerBuilderContext {

private:
	

protected:
	std::vector<ResID>* masterList = new std::vector<ResID>();


	int caseID = 0;
	int sceneID = 0;
	int objectID = 0;
	int shaderID = 0;
	int materialID = 0;
	int meshID = 0;
	int textureID = 0;


	void removeMasterListID(ResID id_p) {
		for (int i = 0; i < masterList->size(); i++)
		{
			if (masterList->at(i) == id_p)
			{
				masterList->erase(masterList->begin() + i);
				break;
			}
		}
	}

	

public:

	std::vector<ResID>* getMasterList() { return masterList; };


	// Items Must be added to the Master List before Their respective add Function!
	void AddIndexToMaster(ResID id) {

		for (int i = 0; i < masterList->size(); i++)
		{
			if (masterList->at(i) == id)
			{
				throw ResourceHandlerIDError("Duplicate ID begin Created!");
			}
		}
		masterList->push_back(id);

	}

	int generateCaseID() {
		caseID += 1;
		return caseID;
	}
	int generateSceneID() {
		sceneID += 1;
		return sceneID;
	}
	int generateObjectID() {
		objectID += 1;
		return objectID;
	}
	int generateMeshID() {
		meshID += 1;
		return meshID;
	}
	int generateMaterialID() {
		materialID += 1;
		return materialID;
	}
	int generateShaderID() {
		shaderID += 1;
		return shaderID;
	}
	int generateTextureID() {
		textureID += 1;
		return textureID;
	}

	


};


class Resource {
protected:
	bool init = false;
	
	virtual void setId(int index) {};

	std::string name = "";

	ResourceHandlerBuilderContext* resourceHandlerCxt;

public:

	ResID _id;

	virtual void build(ResourceHandlerBuilderContext* context) {}

	void setName(std::string name_p) {
		name = name_p;
	}

	std::string getName() {
		return name;
	}


	ResID getId() { return _id; };

	virtual ~Resource() = default;

};


class CaseResource : public Resource {
private:

	std::string* caseFileName = nullptr;

	std::vector<ResID>* Scenes = new std::vector<ResID>();

protected:


public:

	// RESOURCE SPECIFIC FUNCTIONS
	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10000000000 + index;
	}


	CaseResource(ResourceHandlerBuilderContext* context, std::string name_p, std::string filename_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);

		setId(context->generateCaseID());
		context->AddIndexToMaster(getId());

		caseFileName = new std::string(filename_p);

	}

	// CASE FILE NAME CAN BE NULL! NOT ALL CASES ARE SAVED (THOUGHT THAT WAS OBVIOUS)
	std::string* getFileName() { return caseFileName; }


	// CASE METHODS

	std::vector<ResID>* getScenesIdInCase() {
		return Scenes;
	}

	virtual ~CaseResource() = default;


};

class SceneResource : public Resource
{

protected:
	ObjectResource* objectResource;

public:

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10100000000 + index; //  Assigns ID Based on SceneType
	}

	SceneResource(ResourceHandlerBuilderContext* context, std::string name_p ) {
		this->resourceHandlerCxt = context;

		this->setName(name_p);

		setId(context->generateSceneID());
		context->AddIndexToMaster(getId());

	};


	~SceneResource() = default;

};


class ObjectResource : public Resource
{
protected:
	RenderMeshResource* meshResource;

public:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10200000000 + index;
	}

	ObjectResource(ResourceHandlerBuilderContext* context, std::string name_p, RenderMeshResource* meshResource_p) {
		this->resourceHandlerCxt = context;
		this->setName(name_p);
		meshResource = meshResource_p;
		setId(context->generateObjectID());
	}

	virtual ~ObjectResource() = default;

};


enum TextureMapType {
	DIFFUSE_MAP,
	AO_MAP,
	NORMAL_MAP,
	EDGE_MAP,
	HEIGHT_MAP,
	SMOOTHNESS_MAP,
	METALLIC_MAP
};

// hTexture hold the texture image path, loaded texture pointer and the texture position from which ShaderTexture can extract values
struct hTexture {
	std::filesystem::path imagePath;
	
};

// ShaderTexture does not hold the Texture it self but referes to it as multiple maps can be packed and extracted using this method
struct ShaderTexture
{
	std::string textureName = "";
	
	TextureMapType textureType;
	int texturePosition; // Position of texture in Shader

	float scale = 1.0f;
	
};

class ShaderResource : public Resource {
protected:
	std::string ShaderName; 

	ShaderType shaderType;

	//std::vector<ShaderTexture>* ShaderTextures = new std::vector<ShaderTexture>();

	// These are an outward prased view of the shader variables and must not be used to set the shader values themselves 
	// as a shader can be used by multiple Meshes, having different values to the variables!
	std::vector<ShaderVar>* ShaderParameters = new std::vector<ShaderVar>();

public:

	vk::raii::ShaderModule vertexShader = nullptr;
	vk::raii::ShaderModule fragmentShader = nullptr;

	std::filesystem::path vertPath;
	std::filesystem::path fragPath;

	bool isShaderVkLoaded = false;

	bool colorBlending = false;

	vk::PolygonMode polyMode = vk::PolygonMode::eFill;
	vk::CullModeFlagBits culling = vk::CullModeFlagBits::eNone;
	vk::FrontFace frontface = vk::FrontFace::eCounterClockwise;

	MonsterPipe shaderPipes = MonsterPipe();

	vk::DeviceSize pushConstSize = vk::DeviceSize(0);

	vk::raii::DescriptorSets descriptorSets = nullptr;

	std::vector<MonsterTexture> textures{};

	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10700000000 + index + (shaderType * 10000000);
	}

	void _updateDescriptorWrites(
		vk::raii::Device* device,
		const std::vector<MonsterBuffer>& buffer,
		const std::vector<MonsterBuffer>& fragBuf
	) {

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			std::vector<vk::WriteDescriptorSet> descriptorWrites{};

			vk::DescriptorBufferInfo bufferInfo{
				.buffer = buffer.at(i).buffer,
				.offset = vk::DeviceSize(0),
				.range = sizeof(UniformBufferObject)
			};

			vk::DescriptorBufferInfo buffer2Info{
				.buffer = fragBuf.at(i).buffer,
				.offset = vk::DeviceSize(0),
				.range = fragBuf.at(i).bufferSize
			};


			descriptorWrites.push_back({
				.dstSet = descriptorSets.at(i),
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.pBufferInfo = &bufferInfo
				});

			descriptorWrites.push_back(
				{
				.dstSet = descriptorSets[i],
				.dstBinding = 1,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.pBufferInfo = &buffer2Info
				}
			);



			std::vector<vk::DescriptorImageInfo> images{};

			for (size_t i = 0; i < textures.size(); i++)
			{
				vk::DescriptorImageInfo imageInfo{
				.sampler = textures.at(i).textureSampler,
				.imageView = textures.at(i).textureImageView,
				.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
				};

				images.push_back(imageInfo);

			}

			uint32_t texIndex = 2;

			for (size_t img = 0; img < images.size(); img++)
			{
				descriptorWrites.push_back(
					{
					.dstSet = descriptorSets[i],
					.dstBinding = texIndex,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eCombinedImageSampler,
					.pImageInfo = &images.at(img)
					}
				);

				texIndex += 1;
			}

			device->updateDescriptorSets(descriptorWrites, {});

		}

	}


	std::vector<vk::DescriptorSetLayoutBinding> getBindings() {
		std::vector<vk::DescriptorSetLayoutBinding> binds{};
		binds.push_back(vk::DescriptorSetLayoutBinding{
				.binding = 0,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eVertex
			});
		binds.push_back(
			vk::DescriptorSetLayoutBinding{
				.binding = 1,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eFragment
			}
		);

		for (uint32_t i = 0; i < textures.size(); i++)
		{
			binds.push_back(
				vk::DescriptorSetLayoutBinding{
				.binding = 2 + i,
				.descriptorType = vk::DescriptorType::eCombinedImageSampler,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eFragment
				}
			);
		}

		return binds;

	}

	ShaderResource(ResourceHandlerBuilderContext* context, std::string name_p, ShaderType shaderType_p, std::filesystem::path vertFile_p, std::filesystem::path fragFile_p) {
		this->setName(name_p);
		this->resourceHandlerCxt = context;
		shaderType = shaderType_p;
		
		vertPath = vertFile_p;
		fragPath = fragFile_p;

		ShaderName = name_p;
			
		setId(context->generateShaderID());

	}

	void addShaderParameter(ShaderVar variable) {
		ShaderParameters->push_back(variable);	
	}

	std::vector<ShaderVar>* getShaderVars() {
		return ShaderParameters;
	}


	ShaderType getShaderType() {
		return shaderType;
	}

	std::string getShaderName() {
		return ShaderName;
	}

	~ShaderResource() = default;

};


class MaterialResource : public Resource
{

public:

	std::vector<ShaderTexture>* textures;


	bool wireframeMode = false;


	std::string materialFilePath; // file name of material


	void setId(int index) override {
		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10800000000 + index;
	}


	MaterialResource(ResourceHandlerBuilderContext* context, std::string materialName, std::string materialFilePath_p) {
		this->setName(materialName);
		materialFilePath = materialFilePath_p;
		setId(context->generateMaterialID());
	};

	~MaterialResource() = default;

};



class ImageResource : public Resource
{
private:

	std::filesystem::path imagePath;

public:

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10600000000 + index;

	}

	ImageResource(ResourceHandlerBuilderContext* context, std::filesystem::path imagePath_p) {
		imagePath = imagePath_p;

		this->setName(imagePath.filename().string());

		setId(context->generateTextureID());
	}

	std::filesystem::path getImagePath() {
		return imagePath;
	}

};

class RenderMeshResource : public Resource
{

public:
	
	std::vector<MeshData> mesh{};

	bool isMeshVkLoaded = false;

	std::filesystem::path meshFile;

	//std::shared_ptr<vulkanUtils::Shader> shaders;
	std::vector<vk::DeviceSize> allocatingBufferSizes{}; // A default of UniformBuffer is recommanded for adding proper projection and transformation control

	void setId(int index) override {

		if (index > 99999)
		{
			throw ResourceHandlerIDError("Id index exceeds maximum limit of 99,999!");
		}
		_id = 10300000000 + index;

	}

	RenderMeshResource(ResourceHandlerBuilderContext* context, std::string meshName_p, std::filesystem::path meshFile_p) {
		this->resourceHandlerCxt = context;
		this->setName(meshName_p);
		meshFile = meshFile_p;
		setId(context->generateMeshID());
	}

	void setAllocatingBufferInfo(std::vector<vk::DeviceSize> buffers) { allocatingBufferSizes = buffers; }

	virtual void allocateBufferInfo(std::vector<std::vector<MonsterBuffer>>& buffers) {
		throw std::runtime_error("BUFFERS MUST BE ALLOCATED TO HIGHER CLASS!");
	}

	virtual void updateDescriptorWrite(vk::raii::Device* device) {
		throw std::runtime_error("BUFFERS MUST BE ALLOCATED TO HIGHER CLASS!");
	}

	virtual ShaderResource* getShader() { return nullptr; }

	std::string getMeshName() { return name; }

	~RenderMeshResource() = default;

};

// NEW RESOURCES
namespace hRes {

	class Mesh {
	private:
		
		std::shared_ptr<vulkanUtils::Shader> shaders = std::make_shared<vulkanUtils::Shader>();
		std::vector<vk::DeviceSize> allocatingBufferSizes{sizeof(UniformBufferObject)};
	public:

		const char* name;

		Mesh(const char* name_p) { name = name_p; }

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		std::vector<vulkanUtils::Vertex> vertices = std::vector<vulkanUtils::Vertex>();
		std::vector<uint16_t> indices = std::vector<uint16_t>();

		uint32_t vertexBufferIndex = 0; // Most likly they'll both be the same but fuck it let's have em both.
		uint32_t indexBufferIndex = 0;

		bool isMeshVkLoaded = false;

		bool containsPushConstants = false;
		uint32_t pushConstSize = uint32_t(0);

		virtual const void* getPushConst() { return nullptr; }

		std::vector<MonsterBuffer> transformBuffers;

		std::vector<MonsterBuffer> fragBuffers;

		std::vector<vk::raii::DescriptorSets> descritorSets{};

		virtual const std::vector<vk::DeviceSize>& getAllocatingBufferInfo() { return allocatingBufferSizes; }

		void setAllocatingBufferInfo(std::vector<vk::DeviceSize> buffers) { allocatingBufferSizes = buffers; }

		virtual void allocateBufferInfo(std::vector<std::vector<MonsterBuffer>>& buffers) {
			if (buffers.size() != getAllocatingBufferInfo().size())
			{
				throw std::runtime_error("IMPROPER BUFFER ALLOCATION");
			}
			transformBuffers = std::move(buffers.at(0));
			fragBuffers = std::move(buffers.at(1));
		}

		// Used to set gropuing by vulkan to order rendering objects by the pipeline
		//uint32_t graphicsPipelineIndex; // Default pipeline is used when this is null;

		virtual std::weak_ptr<vulkanUtils::Shader> getShader() {
			return shaders;
		}

		virtual void setShader(const std::shared_ptr<vulkanUtils::Shader>& shader) {
			shaders = shader;
		}
		

		virtual void updateDescriptorWrites(vk::raii::Device* device) {
			shaders->_updateDescriptorWrites(device, transformBuffers, descritorSets.front(),fragBuffers, allocatingBufferSizes.back());
		}

		/*void setColor(const glm::vec3& col) {
			colorBufObj.color = col;
			for (auto& colBuf: colorBuffers)
			{
				memcpy(colBuf.allocInfo.pMappedData, &colorBufObj, sizeof(ColorBufferObject));
			}
		}*/

		void updateTransformations(const glm::mat4& view, const glm::mat4& proj) {
			UniformBufferObject ubo{};
			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, position);
			ubo.model = glm::rotate(ubo.model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			ubo.model = glm::rotate(ubo.model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			ubo.model = glm::rotate(ubo.model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.model = glm::scale(ubo.model, scale);
			ubo.view = view;
			ubo.proj = proj;
			ubo.model = glm::transpose(ubo.model);
			ubo.view = glm::transpose(ubo.view);
			ubo.proj = glm::transpose(ubo.proj);
			for (auto& tBuffer: transformBuffers)
			{
				memcpy(tBuffer.allocInfo.pMappedData, &ubo, sizeof(ubo));
			}

		}

		Mesh(const Mesh& mesh) {
			//this->graphicsPipelineIndex = mesh.graphicsPipelineIndex;
			name = mesh.name;
			this->indexBufferIndex = mesh.indexBufferIndex;
			this->vertexBufferIndex - mesh.vertexBufferIndex;
			this->indices = mesh.indices;
			/*this->shaders.fragShaderFilePath = new std::filesystem::path(*mesh.shaders.fragShaderFilePath);
			this->shaders.vertShaderFilePath = new std::filesystem::path(*mesh.shaders.vertShaderFilePath);*/
			this->vertices = mesh.vertices;
		}

	};

	class SampleCube : public Mesh {
	private:
		SampleCube();
	public:
		//std::vector<MonsterBuffer> skyBuffers{};
		ColorBufferObject colBufObj = ColorBufferObject();

		SampleCube(const char* name_p) : Mesh(name_p) {
			setAllocatingBufferInfo({ sizeof(UniformBufferObject), sizeof(ColorBufferObject) });
		}

		void updateDescriptorWrites(vk::raii::Device* device) override {
			getShader().lock()->_updateDescriptorWrites(device, transformBuffers, descritorSets.front(), fragBuffers, sizeof(ColorBufferObject));
			updateBuffer();
		}

		void updateBuffer() {
			for (auto& skyBuf : fragBuffers)
			{
				memcpy(skyBuf.allocInfo.pMappedData, &colBufObj, sizeof(SkyBufferObject));
			}
		}
	};

	class SkyMesh : public Mesh
	{
	private:
		SkyMesh();
	public:

		std::vector<MonsterBuffer> skyBuffers{};
		SkyBufferObject skyBufObj = SkyBufferObject();
		PushConstObject pushConstObj = PushConstObject();

		SkyMesh(const char* name_p) : Mesh(name_p) {
			pushConstObj.time = 0.5f;
			pushConstSize = sizeof(pushConstObj);
			containsPushConstants = true;
			//rotation.y = 90.0f;
			scale = glm::vec3(500.f);
			position = glm::vec3(0, 200.f, 0);
			setAllocatingBufferInfo({ sizeof(UniformBufferObject), sizeof(SkyBufferObject) });

		}

		const void* getPushConst() override { return &pushConstObj; }

		void updateDescriptorWrites(vk::raii::Device* device) override {
			getShader().lock()->_updateDescriptorWrites(device, transformBuffers, descritorSets.front(), fragBuffers, sizeof(SkyBufferObject));
			updateBuffer();
		}

		void updateBuffer() {
			for (auto& skyBuf: fragBuffers)
			{
				memcpy(skyBuf.allocInfo.pMappedData, &skyBufObj, sizeof(SkyBufferObject));
			}
		}
		
	};

	class SkyTexMesh : public Mesh
	{
	private:
		SkyTexMesh();
	public:

		std::vector<MonsterBuffer> skyTexBuffers{};
		SkyTexBufferObject skyTexBufObj = SkyTexBufferObject();
		PushConstObject pushConstObj = PushConstObject();

		SkyTexMesh(const char* name_p) : Mesh(name_p) {
			pushConstObj.time = 0.5f;
			pushConstSize = sizeof(pushConstObj);
			//rotation.y = 90.0f;
			scale = glm::vec3(1300.0f);
			rotation.z = 180.f;
			setAllocatingBufferInfo({ sizeof(UniformBufferObject), sizeof(SkyTexBufferObject) });
			skyTexBufObj.offset = 0.466f;
			skyTexBufObj.smoothness = 0.413f;
		}

		const void* getPushConst() override { return &pushConstObj; }

		void updateDescriptorWrites(vk::raii::Device* device) override {
			getShader().lock()->_updateDescriptorWrites(device, transformBuffers, descritorSets.front(), fragBuffers, sizeof(SkyTexBufferObject));
			updateBuffer();
		}

		void updateBuffer() {
			for (auto& skyBuf : fragBuffers)
			{
				memcpy(skyBuf.allocInfo.pMappedData, &skyTexBufObj, sizeof(SkyTexBufferObject));
			}
		}

	};


}


std::string convertFloatPtrToString(float* val_p, int size);
int convertStringToFloatPtr(std::string str_p,float* &output_p);


std::string convertShaderVarValueToStr(ShaderVar variable);