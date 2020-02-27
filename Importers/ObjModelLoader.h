#pragma once

#include <vector>
#include <fstream>
#include <istream>
#include <d3d11.h>
#include <DirectXMath.h>

// https://www.braynzarsoft.net/viewtutorial/q16390-obj-model-loader

using namespace DirectX;

namespace Kuplung_DX
{
    namespace Importers
    {
        // Texture manager structure, So all the textures are nice and grouped together
        struct TextureManager
        {
            std::vector<ID3D11ShaderResourceView*> TextureList;
            std::vector<std::wstring> TextureNameArray;     // So we don't load in the same texture twice
        };

        struct Vertex
        {
            XMFLOAT3 pos;
            XMFLOAT2 texCoord;
            XMFLOAT3 normal;
            XMFLOAT3 tangent;
            XMFLOAT3 biTangent;
        };

        // Create material structure
        struct SurfaceMaterial
        {
            std::wstring MatName;   // So we can match the subset with it's material

            // Surface's colors
            XMFLOAT4 Diffuse;       // Transparency (Alpha) stored in 4th component
            XMFLOAT3 Ambient;
            XMFLOAT4 Specular;      // Specular power stored in 4th component

            // Texture ID's to look up texture in SRV array
            int DiffuseTextureID;
            int AmbientTextureID;
            int SpecularTextureID;
            int AlphaTextureID;
            int NormMapTextureID;

            // Booleans so we don't implement techniques we don't need
            bool HasDiffTexture;
            bool HasAmbientTexture;
            bool HasSpecularTexture;
            bool HasAlphaTexture;
            bool HasNormMap;
            bool IsTransparent;
        };

        // Model Structure
        struct ObjModel
        {
            int Subsets;                        // Number of subsets in obj model
            ID3D11Buffer* VertBuff;             // Models vertex buffer
            ID3D11Buffer* IndexBuff;            // Models index buffer
            std::vector<XMFLOAT3> Vertices;     // Models vertex positions list
            std::vector<DWORD> Indices;         // Models index list
            std::vector<int> SubsetIndexStart;  // Subset's index offset
            std::vector<int> SubsetMaterialID;  // Lookup ID for subsets surface material
            XMMATRIX World;                     // Models world matrix
            std::vector<XMFLOAT3> AABB;            // Stores models AABB (min vertex, max vertex, and center)
                                                // Where AABB[0] is the min Vertex, and AABB[1] is the max vertex
            XMFLOAT3 Center;                    // True center of the model
            float BoundingSphere;                // Model's bounding sphere
        };

        class ObjModelLoader {
        public:
            bool LoadObjModel(ID3D11Device* device, std::wstring Filename, ObjModel& Model, std::vector<SurfaceMaterial>& material, TextureManager& TexMgr, bool IsRHCoordSys, bool ComputeNormals, bool flipFaces);
        };
    }
}