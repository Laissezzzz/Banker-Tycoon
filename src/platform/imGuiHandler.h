#pragma once

#include <imgui.h>
#include <rlImGui.h>
#include "settings.h"

class ImGuiHandler {
private:
    inline static bool initialized = false;

public:
    static void initialize(const Settings& settings) {
        if (initialized) {
            return;
        }

        rlImGuiSetup(true);

        ImGuiIO& io = ImGui::GetIO();

        if (settings.getImGuiDock()) {
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }

        io.FontGlobalScale = settings.getImGuiFontSize();

        initialized = true;
    }

    static void beginFrame() {
        if (!initialized) {
            return;
        }

        rlImGuiBegin();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{});
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4{});

        ImGui::DockSpaceOverViewport(
            ImGui::GetMainViewport()
        );

        ImGui::PopStyleColor(2);
    }

    static void endFrame() {
        if (!initialized) {
            return;
        }

        rlImGuiEnd();
    }

    static void shutdown() {
        if (!initialized) {
            return;
        }

        rlImGuiShutdown();
        initialized = false;
    }
};