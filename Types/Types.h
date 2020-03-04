#pragma once

#include <vector>
#include <string>

using namespace DirectX;

namespace Kuplung_DX
{
	namespace Types
	{
        typedef enum ViewModelSkin {
            ViewModelSkin_Solid,
            ViewModelSkin_Material,
            ViewModelSkin_Texture,
            ViewModelSkin_Wireframe,
            ViewModelSkin_Rendered
        } ViewModelSkin;

        typedef enum LightSourceType {
            LightSourceType_Directional,
            LightSourceType_Point,
            LightSourceType_Spot
        } LightSourceType;

        typedef enum MaterialTextureType {
            MaterialTextureType_Undefined,
            MaterialTextureType_Ambient,
            MaterialTextureType_Diffuse,
            MaterialTextureType_Dissolve,
            MaterialTextureType_Bump,
            MaterialTextureType_Specular,
            MaterialTextureType_SpecularExp,
            MaterialTextureType_Displacement
        } MaterialTextureType;

        struct PixelDataPoint {
            DirectX::XMFLOAT4 color[4];
            unsigned int index;
            float depth;
        };

        struct ObjectCoordinate {
            bool animate;
            float point;
            ObjectCoordinate() : animate(false), point(0.0f) { }
            ObjectCoordinate(bool a, float p) : animate(a), point(p) { }
        };

        struct ObjectEye {
            DirectX::XMVECTORF32 View_Eye;
            DirectX::XMVECTORF32 View_Center;
            DirectX::XMVECTORF32 View_Up;
        };

        struct MaterialColor {
            bool colorPickerOpen;
            bool animate;
            float strength;
            DirectX::XMFLOAT3 color;
            MaterialColor() : colorPickerOpen(false), animate(false), strength(0.0f), color(DirectX::XMFLOAT3(1, 1, 1)) {}
            MaterialColor(bool cpo, bool a, float s, DirectX::XMFLOAT3 c) : colorPickerOpen(cpo), animate(a), strength(s), color(c) {}
        };

        struct Skybox_Item {
            std::string title;
            std::vector<std::string> images;
        };

        std::string static Kuplung_getTextureName(MaterialTextureType texType) {
            switch (texType) {
            case MaterialTextureType_Ambient:
                return "Ambient";
            case MaterialTextureType_Diffuse:
                return "Diffuse";
            case MaterialTextureType_Dissolve:
                return "Dissolve";
            case MaterialTextureType_Bump:
                return "Normal";
            case MaterialTextureType_Specular:
                return "Specular";
            case MaterialTextureType_SpecularExp:
                return "Specular Exp";
            case MaterialTextureType_Displacement:
                return "Displacement";
            default:
                return "";
            }
        }

        struct MirrorSurface {
            float translateX, translateY, translateZ, rotateX, rotateY, rotateZ;
        };

        struct GUIObjectSetting {
            int oIndex, iValue;
            bool oAnimate, bValue;
            float fValue;
            DirectX::XMFLOAT4 vValue;
        };

        typedef enum LightType {
            LightType_Point = 0,
            LightType_Sun = 1
        } LightType;

        struct LightObject {
            float strength;
            bool colorPickerOpen, doAnimation;
            DirectX::XMFLOAT3 color;
        };

        struct SceneLightCoordinate {
            float coordinate;
            bool doAnimation;
        };

        struct GridMeshPoint2D {
            float x, y;
        };

        struct GridMeshPoint3D {
            float x, y, z;
        };
	}
}