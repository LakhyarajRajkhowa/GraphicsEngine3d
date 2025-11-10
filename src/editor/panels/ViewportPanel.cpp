#include "ViewportPanel.h"

namespace Lengine {

    ViewportPanel::ViewportPanel()
        : m_Framebuffer(1280, 720) // initial size
    {
    }

    void ViewportPanel::OnImGuiRender(Camera3d& camera)
    {
      
       
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      
        
           
          

        ImGui::Begin("Viewport");
        // Track state
        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();

        // Get viewport panel size
        ImVec2 avail = ImGui::GetContentRegionAvail();
        m_ViewportSize = { avail.x, avail.y };

        // Resize framebuffer if needed
        if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
        {
            if (m_ViewportSize != m_LastViewportSize)
            {
                m_Framebuffer.Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
                m_LastViewportSize = m_ViewportSize;
            }
        }

        // Display framebuffer texture
        GLuint texID = m_Framebuffer.GetColorAttachment();

        ImGui::Image(
            (void*)(intptr_t)texID,
            ImVec2(m_ViewportSize.x, m_ViewportSize.y),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();
        ImGui::PopStyleVar();

      
        

    }

}
