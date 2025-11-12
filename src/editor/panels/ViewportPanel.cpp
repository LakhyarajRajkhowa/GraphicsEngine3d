#include "ViewportPanel.h"
#include <iostream>
namespace Lengine {

    ViewportPanel::ViewportPanel()
        : m_Framebuffer(1280, 720) 
    {
    }

    void ViewportPanel::OnImGuiRender(Camera3d& camera)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        m_ViewportPos = ImGui::GetWindowPos();
        m_ViewportSize = ImGui::GetContentRegionAvail();

        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();

        ImVec2 avail = ImGui::GetContentRegionAvail();
        m_ViewportSize = { avail.x, avail.y };

        if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
        {
            if (m_ViewportSize.x != m_LastViewportSize.x ||
                m_ViewportSize.y != m_LastViewportSize.y)
            {
                
                m_LastViewportSize = m_ViewportSize;
            }
        }

        GLuint texID = m_Framebuffer.GetColorAttachment();
        

         ImGui::Image(
             (void*)(intptr_t)texID,
             ImVec2(m_ViewportSize.x, m_ViewportSize.y),
             ImVec2(0, 1),
             ImVec2(1, 0)
         );

         // Get the position of the image itself
         ImVec2 imagePos = ImGui::GetItemRectMin();
         ImVec2 mousePos = ImGui::GetMousePos();


         // So there is an offset error in entity selection due to which
         // the selection ray hits at an offset
         // this is a temporary fix !!!
         // TODO : fix entity selection offset

         float offsetErrorX = offsetValueX * mousePos.x;
         float offsetErrorY = offsetValueY * (m_ViewportSize.y - mousePos.y);
         ImGui::Separator();
         ImGui::Text("🛠 Offset Adjustment");
         ImGui::SliderFloat("Offset X", &offsetValueX, -1.0f, 1.0f, "%.4f");
         ImGui::SliderFloat("Offset Y", &offsetValueY, -1.0f, 1.0f, "%.4f");
         mouseInViewport = {
             mousePos.x - imagePos.x - offsetErrorX,
             mousePos.y - imagePos.y - offsetErrorY
         };
  
        ImGui::End();
        ImGui::PopStyleVar();
    }

}
