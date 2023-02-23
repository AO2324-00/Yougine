#include "ShaderGraphWindow.h"

namespace editor::shadergraph
{
    ShaderGraphWindow::ShaderGraphWindow(EditorWindowsManager* editor_windows_manager) : EditorWindow(editor_windows_manager, editor::EditorWindowName::ShaderGraphWindow)
    {
        InitializeMenuLists();
    }

    void ShaderGraphWindow::InitializeMenuLists()
    {
        menu_bar_list =
        {
            "Add Node",
        };

        menu_item_list =
        {
            "Sample Node",
        };
    }


    void ShaderGraphWindow::Draw()
    {
        ImGuiWindowFlags flags = (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
        ImGui::Begin("simple node editor", nullptr, flags);

        //���j���[�o�[
        EditorWindow::RenderMenuBar();

        ImNodes::BeginNodeEditor();

        //�m�[�h��`��
        PhaseNode();

        //�m�[�h���m���q���郊���N����`��
        PhaseLink();

        ImNodes::EndNodeEditor();

        PhaseAddLink();
        PhaseDisLink();

        ImGui::End();
    }

    void ShaderGraphWindow::SelectedItemProcess(std::string item)
    {
        if (ImGui::MenuItem(item.c_str()))
        {
            int id = nodes.empty() ? 1 : nodes.back()->output_attrs.back() + 1;
            AddNode(id, 1, 1, item);
        }
    }


    void ShaderGraphWindow::PhaseNode()
    {
        for (ShaderGraphNode* node : nodes)
        {
            DrawNode(node);
        }
    }

    /*
     * �`�悷��m�[�h�𑝂₵�����Ƃ��ɌĂяo��
     *
     * id ... �O�m�[�h��output_attr.back()+1�̒l��n��
     * num_inputs ... input_attr�̐�
     * num_outputs ... ouput_attr�̐�
     */
    void ShaderGraphWindow::AddNode(int id, int num_inputs, int num_outputs, std::string name)
    {
        ShaderGraphNode* node = new ShaderGraphNode();
        node->id = id;

        for (int inputID = node->id + 1; inputID < num_inputs + node->id + 1; inputID++)
        {
            node->input_attrs.push_back(inputID);
        }

        for (int outputID = node->input_attrs.back() + 1; outputID < num_outputs + node->input_attrs.back() + 1; outputID++)
        {
            node->output_attrs.push_back(outputID);
        }
        node->name = name;

        nodes.push_back(node);
        std::cout << node->id << std::endl;
    }

    /*
     * nodes�z��Ɋi�[����Ă���Node�\���̂�Node�Ƃ��ĕ`�悷��
     */
    void ShaderGraphWindow::DrawNode(ShaderGraphNode* node)
    {
        ImNodes::BeginNode(node->id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node->name.c_str());
        ImNodes::EndNodeTitleBar();

        for (int inputID : node->input_attrs)
        {
            ImNodes::BeginInputAttribute(inputID);
            ImGui::Text("input");
            ImNodes::EndInputAttribute();
        }

        for (int outputID : node->output_attrs)
        {
            ImNodes::BeginOutputAttribute(outputID);
            ImGui::Indent(40);
            ImGui::Text("output");
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }


    /*
     * Node���m�������N����
     * links���X�g�Ɋi�[����Ă��郊���N�\���̑S�Ă������N��ԂƂ��ĕ`�悷��
     */
    void ShaderGraphWindow::PhaseLink()
    {
        for (Link& link : links)
        {
            ImNodes::Link(link.id, link.start_attr, link.end_attr);
        }
    }

    /*
     * �����N�\���̂�ǉ�����
     * �m�[�h1����m�[�h2�̃s���Ƀ����N�����킹���炻���������N���Ă���Ƃ݂Ȃ��A�����N�\���̂𐶐�����links���X�g�ɒǉ�����
     */
    void ShaderGraphWindow::PhaseAddLink()
    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            //�����N����m�[�h��T��
            std::pair<ShaderGraphNode*, ShaderGraphNode*> sub_nodes; //0��input, 1��output
            int input_attr = link.end_attr, output_attr = link.start_attr;

            bool is_detected_input_node = false, is_detected_output_node = false;
            //int first_attr_id = input_attr < output_attr ? 0 : 1;
            for (ShaderGraphNode* node : nodes)
            {
                if (!is_detected_input_node)
                {
                    for (int i_attr : node->input_attrs)
                    {
                        is_detected_input_node = (i_attr == input_attr);
                        if (is_detected_input_node)
                        {
                            sub_nodes.first = node;
                            break;
                        }
                    }
                }
                if (!is_detected_output_node)
                {
                    for (int o_attr : node->output_attrs)
                    {
                        is_detected_output_node = (o_attr == output_attr);
                        if (is_detected_output_node)
                        {
                            sub_nodes.second = node;
                            break;
                        }
                    }
                }
            }
            std::pair < std::pair<ShaderGraphNode*, ShaderGraphNode*>, std::pair<int, int>> linked_pair = { sub_nodes, std::pair<int, int>{input_attr, output_attr} };
            linked_nodes_pair.push_back(linked_pair);

            UpdateLinkedNodesValue();

            std::cout << "LinkCreated:" + std::to_string(link.start_attr) + "to" + std::to_string(link.end_attr) << std::endl;
            link.id = ++currentLinks;
            links.push_back(link);
        }
    }

    /*
     * �����N����������
     * �����N���Ă���m�[�h�̃s�����h���b�O�A���h�h���b�v�����炻�̃����N����������
     */
    void ShaderGraphWindow::PhaseDisLink()
    {
        int pin;
        //�m�[�h�̃s���Ƀ}�E�X�z�o�[����Ă����Ԃō��N���b�N
        if (ImNodes::IsPinHovered(&pin) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            std::cout << "PinClicked:" + std::to_string(pin) << std::endl;
            //�I������Ă��郊���N������
            int l_id;
            if (ImNodes::IsLinkHovered(&l_id))
            {
                std::cout << "LinkDestroyed" << std::endl;
                //ImNodes::ClearLinkSelection();

                //�������ꂽ�����N��������links���X�g�ɍX�V
                std::vector<Link> newLinks;
                for (Link ll : links)
                {
                    if (ll.id != l_id) newLinks.push_back(ll);
                }
                links = newLinks;
            }
        }
    }

    void ShaderGraphWindow::SendOutputValToInput(std::pair<ShaderGraphNode*, ShaderGraphNode*> sub_nodes, int input_attr, int output_attr)
    {
        sub_nodes.first->UpdateVal(sub_nodes.second, input_attr, output_attr);
        //sub_nodes.first->SetInputVal(sub_nodes.second->GetOutputVal(output_attr), input_attr);
    }

    void ShaderGraphWindow::UpdateLinkedNodesValue()
    {
        for (std::pair<std::pair<ShaderGraphNode*, ShaderGraphNode*>, std::pair<int, int>> p_nodes : linked_nodes_pair)
        {
            SendOutputValToInput(p_nodes.first, p_nodes.second.first, p_nodes.second.second);
            p_nodes.first.first->DisplayValues();
            p_nodes.first.second->DisplayValues();
        }
    }


}
