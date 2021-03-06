/**
 * @file   GPUProgram.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.12.14
 *
 * @brief  Declaration of a GPU program resource class.
 */

#pragma once

#include "core/resources/Resource.h"
#include "core/open_gl_fwd.h"
#include "core/utils/function_view.h"

namespace viscom {

    class ApplicationNodeInternal;
    class Shader;

    /**
     * Complete GPU program with multiple Shader objects working together.
     */
    class GPUProgram final : public Resource
    {
    public:
        GPUProgram(const std::string& programName, ApplicationNodeInternal* node, bool synchronize);
        GPUProgram(const GPUProgram& orig) = delete;
        GPUProgram& operator=(const GPUProgram&) = delete;
        GPUProgram(GPUProgram&&) noexcept = delete;
        GPUProgram& operator=(GPUProgram&&) noexcept = delete;
        virtual ~GPUProgram() noexcept override;

        [[deprecated("Use the vector version instead.")]]
        void Initialize(std::initializer_list<std::string> shaderNames);
        void Initialize(std::vector<std::string> shaderNames);
        void Initialize(std::vector<std::string> shaderNames, const std::vector<std::string>& defines);

        [[deprecated("Use 'Load' (without optional parameter) instead.")]]
        void recompileProgram();
        /** Returns the OpenGL program id. */
        GLuint getProgramId() const noexcept { return program_; }
        /** Returns a uniform locations. */
        GLint getUniformLocation(const std::string& name) const;
        /** Returns a list of uniform locations. */
        [[deprecated("Use the vector version instead.")]]
        std::vector<GLint> getUniformLocations(const std::initializer_list<std::string>& names) const;
        std::vector<GLint> GetUniformLocations(const std::vector<std::string>& names) const;

        /** Returns a attribute locations. */
        GLint getAttributeLocation(const std::string& name) const;
        /** Returns a list of attribute locations. */
        [[deprecated("Use the vector version instead.")]]
        std::vector<GLint> getAttributeLocations(const std::initializer_list<std::string>& names) const;
        std::vector<GLint> GetAttributeLocations(const std::vector<std::string>& names) const;

    protected:
        virtual void Load(std::optional<std::vector<std::uint8_t>>& data) override;
        virtual void LoadFromMemory(const void* data, std::size_t size) override;

    private:
        using ShaderList = std::vector<std::unique_ptr<Shader>>;

        /** Holds the program name. */
        std::string programName_;
        /** Holds the shader names. */
        std::vector<std::string> shaderNames_;
        /** Holds the program. */
        GLuint program_;
        /** Holds a list of shaders used internally. */
        ShaderList shaders_;
        /** Holds the defines used in the program. */
        std::vector<std::string> defines_;

        template<typename T, typename SHAcc> static GLuint linkNewProgram(const std::string& name,
            const std::vector<T>& shaders, SHAcc shaderAccessor);
        void LoadProgram(viscom::function_view<std::unique_ptr<Shader>(const std::string&, const ApplicationNodeInternal*)> createShader);
    };
}
