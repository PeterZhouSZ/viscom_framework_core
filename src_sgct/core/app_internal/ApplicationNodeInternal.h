/**
 * @file   ApplicationNodeInternal.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declares a base class for all application nodes in the VISCOM lab cluster.
 */

#pragma once

#include "core/main.h"
#ifdef VISCOM_SYNCINPUT
#include "core/InputWrapper.h"
#endif
#include "core/FrameworkInternal.h"
#include "sgct/SharedDataTypes.h"

namespace viscom::tuio {
    class TuioInputWrapper;
}

namespace viscom {

    class ApplicationNodeBase;

    struct InternalSyncedInfo {
        double currentTime_ = 0.0;
        glm::vec3 cameraPosition_;
        glm::quat cameraOrientation_;
        glm::mat4 pickMatrix_;
    };

    class ApplicationNodeInternal
    {
    public:
        ApplicationNodeInternal(FrameworkInternal& fwInternal);
        ApplicationNodeInternal(const ApplicationNodeInternal&) = delete;
        ApplicationNodeInternal(ApplicationNodeInternal&&) = delete;
        ApplicationNodeInternal& operator=(const ApplicationNodeInternal&) = delete;
        ApplicationNodeInternal& operator=(ApplicationNodeInternal&&) = delete;
        virtual ~ApplicationNodeInternal();

        virtual void PreWindow();
        virtual void InitOpenGL();
        virtual void PreSync();
        virtual void PostSync();
        virtual void ClearBuffer(FrameBuffer& fbo);
        virtual void DrawFrame(FrameBuffer& fbo);
        virtual void Draw2D(FrameBuffer& fbo);
        virtual void PostDraw();
        virtual void CleanUp();
        void DataTransfer(void* receivedData, int receivedLength, std::uint16_t packageID, int clientID);
        void DataAcknowledge(std::uint16_t packageID, int clientID);
        void DataTransferStatus(bool connected, int clientID);
        void EncodeData();
        void DecodeData();

        virtual void KeyboardCallback(int key, int scancode, int action, int mods);
        virtual void CharCallback(unsigned int character, int mods);
        virtual void MouseButtonCallback(int button, int action);
        virtual void MousePosCallback(double x, double y);
        virtual void MouseScrollCallback(double xoffset, double yoffset);
        virtual void AddTuioCursor(TUIO::TuioCursor *tcur);
        virtual void UpdateTuioCursor(TUIO::TuioCursor *tcur);
        virtual void RemoveTuioCursor(TUIO::TuioCursor *tcur);

        virtual bool InitialiseVR();
        virtual bool CalibrateVR(CalibrateMethod method, TrackedDeviceIdentifier trackedDevice);
        virtual glm::vec3 GetControllerPosition(TrackedDeviceIdentifier trackedDevice);
        virtual glm::vec3 GetControllerZVector(TrackedDeviceIdentifier trackedDevice);
        virtual glm::quat GetControllerRotation(TrackedDeviceIdentifier trackedDevice);
        virtual glm::vec2 GetDisplayPosition(TrackedDeviceIdentifier trackedDevice);

        virtual void ControllerButtonPressedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, float posx, float posy, glm::vec3 position, glm::vec3 zvector, glm::quat rotation);
        virtual void ControllerButtonTouchedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, float posx, float posy, glm::vec3 position, glm::vec3 zvector, glm::quat rotation);
        virtual void ControllerButtonUnpressedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, float posx, float posy, glm::vec3 position, glm::vec3 zvector, glm::quat rotation);
        virtual void ControllerButtonUntouchedCallback(TrackedDeviceIdentifier trackedDevice, ControllerButtonIdentifier buttonid, float posx, float posy, glm::vec3 position, glm::vec3 zvector, glm::quat rotation);

        virtual void ParseTrackingFrame();
        virtual glm::vec3 GetController0Pos();
        virtual glm::vec3 GetController0Zvec();
        virtual glm::vec3 GetController1Pos();
        virtual glm::vec3 GetController1Zvec();
        virtual glm::vec3 GetTrackerPos();
        virtual glm::vec3 GetTrackerZvec();
        virtual glm::quat GetController0Rot();
        virtual glm::quat GetController1Rot();
        virtual glm::quat GetTrackerRot();
        virtual glm::vec2 GetDisplayPosition(bool useleftcontroller);
        virtual void InitialiseDisplay(bool useLeftController);
        virtual bool GetDisplayInitialised();
        virtual void SetDisplayNotInitialised();
        virtual bool GetDisplayInitByFloor();
        virtual void SetDisplayInitByFloor(bool b);
        virtual void PollAndParseNextEvent();
        virtual void PollAndParseEvents();
        virtual std::vector<std::string> OutputDevices();
        virtual float* GetDisplayEdges();
        virtual bool GetVrInitSuccess() = 0;
        virtual std::vector<std::string> GetController0Buttons() = 0;
        virtual std::vector<std::string> GetController1Buttons() = 0;

        double GetCurrentAppTime() const { return syncInfoLocal_.currentTime_; }
        double GetElapsedTime() const { return elapsedTime_; }

        FrameworkInternal & GetFramework() { return fwInternal_; }

    protected:
        void SetApplicationNode(std::unique_ptr<ApplicationNodeBase> appNodeImpl) { appNodeImpl_ = std::move(appNodeImpl); }

        /** Holds the synchronized object (local). */
        InternalSyncedInfo syncInfoLocal_;
        /** Holds the synchronized object (synced). */
        sgct::SharedObject<InternalSyncedInfo> syncInfoSynced_;

        

#ifdef VISCOM_SYNCINPUT
        /** Holds the synchronized vector with keyboard events. */
        sgct::SharedVector<KeyboardEvent> keyboardEventsSynced_;
        /** Holds the synchronized vector with character events. */
        sgct::SharedVector<CharEvent> charEventsSynced_;
        /** Holds the synchronized vector with mouse button events. */
        sgct::SharedVector<MouseButtonEvent> mouseButtonEventsSynced_;
        /** Holds the synchronized vector with mouse position events. */
        sgct::SharedVector<MousePosEvent> mousePosEventsSynced_;
        /** Holds the synchronized vector with mouse scroll events. */
        sgct::SharedVector<MouseScrollEvent> mouseScrollEventsSynced_;
#endif

    private:
        /** The internal framework class. */
        FrameworkInternal& fwInternal_;
        /** Holds the application node implementation. */
        std::unique_ptr<ApplicationNodeBase> appNodeImpl_;
        /** The input wrapper for TUIO. */
        std::unique_ptr<tuio::TuioInputWrapper> tuio_;

        /** Holds the last frame time. */
        double lastFrameTime_ = 0.0;
        /** Holds the time elapsed since the last frame. */
        double elapsedTime_;

        struct ResourceData {
            ResourceType type_ = ResourceType::All_Resources;
            std::string name_;
            std::vector<std::uint8_t> data_;

            bool operator==(const ResourceData& other) const { return type_ == other.type_ && name_ == other.name_; }
        };

        /** Synchronized resources to be created at next possible time. */
        std::vector<ResourceData> creatableResources_;
        /** The mutex for creatable resources. */
        std::mutex creatableResourceMutex_;

        
    };
}
