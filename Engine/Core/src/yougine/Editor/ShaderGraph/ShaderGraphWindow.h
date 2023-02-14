#pragma once
#include "imnodes/imnodes.h"
#include "imgui/imgui.h"
#include "../EditorWindow.h"

namespace editor::shadergraph
{
    /*
     * �����N�\����
     */
    struct Link
    {
        int id; //�����N�ŗLID
        int start_attr, end_attr; //�n�_ID, �I�_ID 
    };

    class ShaderGraphWindow : public EditorWindow
    {
    private:
        std::vector<Link> links; //���̃��X�g�Ɋi�[����Ă��郊���N�\���̂̃����N��`��
        int currentLinks = 0; //Link.id�̎w�W

    private:
        void SearchSelectingNodeID();
        void PhaseLink();
        void PhaseAddLink();
        void PhaseDisLink();

    public:
        ShaderGraphWindow(EditorWindowsManager* editor_windows_manager);
        void Draw() override;
    };
}
