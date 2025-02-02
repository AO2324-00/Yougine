#pragma once
#include "imnodes/imnodes.h"
#include "imgui/imgui.h"
#include "../EditorWindow.h"
#include "Nodes/ShaderGraphNode.h"
#include "Nodes/ShaderGraphInputNodes.h"
#include "Nodes/UnlitShaderGraphNode.h"
#include "../../utilitys/Split.cpp"
#include "ShaderfileOverwriter.h"

namespace editor::shadergraph
{
    /*
     * リンク構造体
     */
    struct Link
    {
        int id; //リンク固有ID
        int start_attr, end_attr; //始点ID, 終点ID 
    };

    class ShaderGraphWindow : public EditorWindow
    {
    private:
        std::vector<ShaderGraphNode*> nodes;
        std::vector<Link> links; //このリストに格納されているリンク構造体のリンクを描画
        int currentLinks = 0; //Link.idの指標
        ShaderfileOverwriter* shaderfile_overwriter;

    private:
        void SearchSelectingNodeID();
        void PhaseLink();
        void PhaseAddLink();
        void PhaseDisLink();
        void PhaseNode();
        void AddNode(ShaderGraphNode* node, int id, int num_inputs, int num_outputs, std::string name);
        void DrawNode(ShaderGraphNode* node);
        void UpdateLinkedNode(ShaderGraphNode* parent_node, ShaderGraphNode* child_node, std::pair<int, int> attr_pair);
        void UpdateNodeValue(ShaderGraphNode* child_node, std::pair<int, int> attr_pair);
        void DisLinkNodes(std::pair<int, int> link_pair);
        std::pair<ShaderGraphNode*, ShaderGraphNode*> FindSubNodesByLinkAttr(std::pair<int, int> attr_pair);

    public:
        ShaderGraphWindow(EditorWindowsManager* editor_windows_manager);
        void Draw() override;

    protected:
        void SelectedItemProcess(std::string item) override;
        void InitializeMenuLists() override;
    };
}
