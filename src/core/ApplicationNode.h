/**
 * @file   ApplicationNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declares a base class for all application nodes in the VISCOM lab cluster.
 */

#pragma once

#include "main.h"
#include "sgct.h"
#include "InputWrapper.h"
#include <mutex>

namespace viscom {

    class ApplicationNodeImplementation;

    class ApplicationNode
    {
    public:
        ApplicationNode(FWConfiguration&& config, std::unique_ptr<sgct::Engine> engine);
        ApplicationNode(const ApplicationNode&) = delete;
        ApplicationNode(ApplicationNode&&) = delete;
        ApplicationNode& operator=(const ApplicationNode&) = delete;
        ApplicationNode& operator=(ApplicationNode&&) = delete;
        virtual ~ApplicationNode();

        void InitNode();
        void Render() const;

        void BasePreWindow();
        void BaseInitOpenGL();
        void BasePreSync();
        void PostSyncFunction();
        void BaseClearBuffer() const;
        void BaseDrawFrame() const;
        void BaseDraw2D() const;
        void BasePostDraw() const;
        void BaseCleanUp() const;

        void BaseKeyboardCallback(int key, int scancode, int action, int mods);
        void BaseCharCallback(unsigned int character, int mods);
        void BaseMouseButtonCallback(int button, int action);
        void BaseMousePosCallback(double x, double y);
        void BaseMouseScrollCallback(double xoffset, double yoffset);

        static void BaseEncodeDataStatic();
        static void BaseDecodeDataStatic();
        void BaseEncodeData();
        void BaseDecodeData();

        sgct::Engine* GetEngine() const { return engine_.get(); }
        const FWConfiguration& GetConfig() const { return config_; }
        unsigned int GetGlobalProjectorId(int nodeId, int windowId) const;
        const std::pair<glm::ivec2, glm::ivec2>& GetViewport(size_t windowId) const { return viewport_[windowId]; }
        std::pair<glm::ivec2, glm::ivec2>& GetViewport(size_t windowId) { return viewport_[windowId]; }
        const glm::vec2& GetViewportScaling(size_t windowId) const { return viewportScaling_[windowId]; }
        glm::vec2& GetViewportScaling(size_t windowId) { return viewportScaling_[windowId]; }
        const glm::ivec2& GetViewportOrigin(size_t windowId) const { return viewportOrigin_[windowId]; }
        glm::ivec2& GetViewportOrigin(size_t windowId) { return viewportOrigin_[windowId]; }
        const glm::ivec2& GetViewportSize(size_t windowId) const { return viewportSize_[windowId]; }
        glm::ivec2& GetViewportSize(size_t windowId) { return viewportSize_[windowId]; }

    private:
        void loadProperties();

        /** Holds a static pointer to an object to this class making it singleton in a way. */
        // TODO: This is only a workaround and should be fixed in the future. [12/5/2016 Sebastian Maisch]
        static ApplicationNode* instance_;
        /** Holds the mutex for the instance pointer. */
        static std::mutex instanceMutex_;

        /** Holds the applications configuration. */
        FWConfiguration config_;
        /** Holds the application node implementation. */
        std::unique_ptr<ApplicationNodeImplementation> appNodeImpl_;
        /** Holds the SGCT engine. */
        std::unique_ptr<sgct::Engine> engine_;
        /** Holds the start node used for slaves. */
        unsigned int startNode_;
        /** Holds the masters port. */
        std::string masterSocketPort_;

        /** Holds the viewport for rendering content to each window. */
        std::vector<std::pair<glm::ivec2, glm::ivec2>> viewport_;
        /** Holds the viewport scaling if one applies. */
        std::vector<glm::vec2> viewportScaling_;
        /** Holds the viewport origin if exists. */
        std::vector<glm::ivec2> viewportOrigin_;
        /** Holds the viewport scaling if one applies. */
        std::vector<glm::ivec2> viewportSize_;

        /** Holds the synchronized application time. */
        sgct::SharedDouble currentTimeSynced_;
        /** Holds the current application time. */
        double currentTime_;

        /** Holds the vector with keyboard events. */
        std::vector<KeyboardEvent> keyboardEvents_;
        /** Holds the synchronized vector with keyboard events. */
        sgct::SharedVector<KeyboardEvent> keyboardEventsSynced_;
        /** Holds the vector with character events. */
        std::vector<CharEvent> charEvents_;
        /** Holds the synchronized vector with character events. */
        sgct::SharedVector<CharEvent> charEventsSynced_;
        /** Holds the vector with mouse button events. */
        std::vector<MouseButtonEvent> mouseButtonEvents_;
        /** Holds the synchronized vector with mouse button events. */
        sgct::SharedVector<MouseButtonEvent> mouseButtonEventsSynced_;
        /** Holds the vector with mouse position events. */
        std::vector<MousePosEvent> mousePosEvents_;
        /** Holds the synchronized vector with mouse position events. */
        sgct::SharedVector<MousePosEvent> mousePosEventsSynced_;
        /** Holds the vector with mouse scroll events. */
        std::vector<MouseScrollEvent> mouseScrollEvents_;
        /** Holds the synchronized vector with mouse scroll events. */
        sgct::SharedVector<MouseScrollEvent> mouseScrollEventsSynced_;
    };
}
