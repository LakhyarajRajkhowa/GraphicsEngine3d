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

        ImVec2 avail = ImGui::GetContentRegionAvail();
        m_ViewportSize = { avail.x, avail.y };
        m_ViewportPos = ImGui::GetWindowPos();

        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();

      //  ImVec2 avail = ImGui::GetContentRegionAvail();
       // m_ViewportSize = { avail.x, avail.y };

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

         ImVec2 imagePos = ImGui::GetItemRectMin();
         ImVec2 mousePos = ImGui::GetMousePos();
  

         mouseInViewport = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
    

        ImGui::End();
        ImGui::PopStyleVar();
    }

}
