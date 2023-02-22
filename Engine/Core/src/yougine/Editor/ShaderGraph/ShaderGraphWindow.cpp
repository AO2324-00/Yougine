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

        //メニューバー
        EditorWindow::RenderMenuBar();

        ImNodes::BeginNodeEditor();

        //ノードを描画
        PhaseNode();

        //ノード同士を繋げるリンク線を描画
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
     * 描画するノードを増やしたいときに呼び出す
     *
     * id ... 前ノードのoutput_attr.back()+1の値を渡す
     * num_inputs ... input_attrの数
     * num_outputs ... ouput_attrの数
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
     * nodes配列に格納されているNode構造体をNodeとして描画する
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
     * Node同士をリンクする
     * linksリストに格納されているリンク構造体全てをリンク状態として描画する
     */
    void ShaderGraphWindow::PhaseLink()
    {
        for (Link& link : links)
        {
            ImNodes::Link(link.id, link.start_attr, link.end_attr);
        }
    }

    /*
     * リンク構造体を追加する
     * ノード1からノード2のピンにリンクを合わせたらそれらをリンクしているとみなし、リンク構造体を生成してlinksリストに追加する
     */
    void ShaderGraphWindow::PhaseAddLink()
    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            std::cout << "LinkCreated:" + std::to_string(link.start_attr) + "to" + std::to_string(link.end_attr) << std::endl;
            link.id = ++currentLinks;
            links.push_back(link);
        }
    }

    /*
     * リンクを解除する
     * リンクしているノードのピンをドラッグアンドドロップしたらそのリンクを解除する
     */
    void ShaderGraphWindow::PhaseDisLink()
    {
        int pin;
        //ノードのピンにマウスホバーされている状態で左クリック
        if (ImNodes::IsPinHovered(&pin) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            std::cout << "PinClicked:" + std::to_string(pin) << std::endl;
            //選択されているリンクを解除
            int l_id;
            if (ImNodes::IsLinkHovered(&l_id))
            {
                std::cout << "LinkDestroyed" << std::endl;
                //ImNodes::ClearLinkSelection();

                //解除されたリンクを除いたlinksリストに更新
                std::vector<Link> newLinks;
                for (Link ll : links)
                {
                    if (ll.id != l_id) newLinks.push_back(ll);
                }
                links = newLinks;
            }
        }
    }

    void ShaderGraphWindow::SendOutputValToInput(int value, int input_attr, int output_attr)
    {
        ShaderGraphNode* sub_nodes[2]; //0がinput, 1がoutput

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
                        sub_nodes[0] = node;
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
                        sub_nodes[1] = node;
                        break;
                    }
                }
            }
        }

        sub_nodes[0]->SetInputVal(sub_nodes[1]->GetOutputVal(output_attr), input_attr);
    }

}
