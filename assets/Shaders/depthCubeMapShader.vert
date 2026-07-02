#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

uniform mat4 model;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool useSkeleton;

void main()
{
    mat4 skinMatrix = mat4(1.0);

    if(useSkeleton)
    {
        bool validBone = false;
        for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            if(boneIds[i] >= 0 && boneIds[i] < MAX_BONES && weights[i] > 0.0)
                validBone = true;
        }

        if(validBone)
        {
            skinMatrix = mat4(0.0);
            for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
            {
                if(boneIds[i] >= 0 && boneIds[i] < MAX_BONES)
                    skinMatrix += finalBonesMatrices[boneIds[i]] * weights[i];
            }
        }
    }

    vec4 skinnedPosition = skinMatrix * vec4(aPos, 1.0);

    // NOTE: no projection here — geometry shader applies shadowMatrices[face] per-face
    gl_Position = model * skinnedPosition;
}