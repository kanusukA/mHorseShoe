#include "Mediator.h"

SelectedCase* ModelComponent::selectedCase;
SelectedMaterial* ModelComponent::selectedMaterial;
SelectedScene* ModelComponent::selectedScene;
SelectedObject* ModelComponent::selectedObject;

std::vector<std::filesystem::path>* ModelComponent::meshDpVec;
std::vector<std::filesystem::path>* ModelComponent::materialDpVec;
std::vector<std::filesystem::path>* ModelComponent::shaderDpVec;
std::vector<std::filesystem::path>* ModelComponent::textureDpVec;

std::vector<std::shared_ptr<Case>>* ModelComponent::caseVec;

std::vector<std::filesystem::path>* ModelComponent::savedCaseFiles;