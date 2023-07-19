#include "light.h"
#include "../resource-manager.h"
#include <vector>
#include <string>

Shader Light::shader;

Light::Light(glm::vec2 pos, float range)
{
	this->pos = pos;
	this->range = range;
}

void Light::setup()
{
	const std::string vert_src =
	#include "../shaders/debug.vert"
	;

	const std::string frag_src =
	#include "../shaders/debug.frag"
	;

	/* Load the shader program and save it */
	shader = ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "debug");
}

// TEMP: for testing.
const std::vector<glm::vec2> wall_vertices = {
	{ -1.0f, -2.0f },
	{  1.0f, -2.0f },
	{ -1.0f, 1.0f },
	{  1.0f, 1.0f }
};

void Light::compute()
{

}

void Light::draw()
{
    /*
        1. Enable stencil testing.
        2. Draw the shadow mask onto the stencil buffer.
        3. Draw the light. (now being masked by the stencil buffer)
        4. Disable the stencil testing.
    */

    /* Enable the stencil testing */
    glEnable(GL_STENCIL_TEST);

    /* Draw the shadow mask onto the stencil buffer */
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    glDrawArrays(GL_TRIANGLES, 36, 6);

    /* Draw the light with respect to the shadow mask */
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer

    /*model = glm::scale(
        glm::translate(model, glm::vec3(0, 0, -1)),
        glm::vec3(1, 1, -1)
    );
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);*/

    /* Disable the stencil testing */
    glDisable(GL_STENCIL_TEST);
}
