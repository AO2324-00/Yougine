///*
// * main.cpp�R�����g�A�E�g���Ă���̃R�����g�A�E�g�O���Ɠ���
// *
// * 1. JSON�t�@�C����ǂݍ���
// * 2. �V�[���̃C���X�^���X���쐬
// * 3. �Q�[���I�u�W�F�N�g�C���X�^���X���쐬
// *      3.1. �e�Q�[���I�u�W�F�N�g�������Ă���R���|�[�l���g�C���X�^���X���쐬
// *          3.1.1. �e�R���|�[�l���g�������Ă���v���p�e�B�̒l���Z�b�g
// *
// *  ����̃e�X�g�R�[�h�ł͖��O��"testgameobject"�̃Q�[���I�u�W�F�N�g������DebugComponent�̃v���p�e�B���o�͂��Ă���
// */
//
//#include "SceneFiles/SceneLoader.h"
//
//int main()
//{
//    std::string filepath = "./testScene.json";
//
//    yougine::SceneFiles::SceneLoader* scene_loader = new yougine::SceneFiles::SceneLoader();
//
//    scene_loader->UpdateJsonObj(filepath);
//    scene_loader->CreateScene();
//
//    yougine::GameObject* obj = scene_loader->jb_scene->GetGameObjectByName("testgameobject1");
//    yougine::components::Component* component = obj->GetComponent<yougine::components::DebugComponent>();
//
//    std::vector<std::vector<std::any>> ap = component->GetAccessablePropertiesList();
//
//    std::cout << std::any_cast<int>(ap[0][0]) << std::endl;
//    std::cout << std::any_cast<int>(ap[1][0]) << std::endl;
//    std::cout << std::any_cast<float>(ap[2][0]) << std::endl;
//    //std::cout << std::any_cast<utility::Vector3>(ap[3][0]) << std::endl;
//    std::cout << std::any_cast<std::string>(ap[4][0]) << std::endl;
//    std::cout << std::any_cast<bool>(ap[5][0]) << std::endl;
//}
