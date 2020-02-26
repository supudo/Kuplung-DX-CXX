#pragma once

//#include "pch.h"
#include <vector>
#include "Utilities\CXXUtils.h"

using namespace DirectX;

namespace Kuplung_DX
{
	namespace Models
	{
		typedef enum objMaterialImageType {
			objMaterialImageType_Ambient,
			objMaterialImageType_Diffuse,
			objMaterialImageType_Specular,
			objMaterialImageType_SpecularExp,
			objMaterialImageType_Dissolve,
			objMaterialImageType_Bump,
			objMaterialImageType_Displacement
		} objMaterialImageType;

		struct MeshMaterialTextureImage {
			int Width, Height;
			bool UseTexture;
			char mmtiPad[7];
			std::string Filename;
			std::string Image;
			std::vector<std::string> Commands;
		};

		struct MeshModelMaterial {
			int MaterialID;
			float SpecularExp;
			std::string MaterialTitle;

			DirectX::XMFLOAT3 AmbientColor, DiffuseColor, SpecularColor, EmissionColor;
			float Transparency;
			unsigned int IlluminationMode;
			float OpticalDensity;

			MeshMaterialTextureImage TextureAmbient;
			MeshMaterialTextureImage TextureDiffuse;
			MeshMaterialTextureImage TextureSpecular;
			MeshMaterialTextureImage TextureSpecularExp;
			MeshMaterialTextureImage TextureDissolve;
			MeshMaterialTextureImage TextureBump;
			MeshMaterialTextureImage TextureDisplacement;
		};

		struct MeshModel {
			int ID;
			std::string File;
			std::string ModelTitle, MaterialTitle;
			int countVertices, countTextureCoordinates, countNormals, countIndices;

			MeshModelMaterial ModelMaterial;
			std::vector<DirectX::XMFLOAT3> vertices;
			std::vector<DirectX::XMFLOAT2> texture_coordinates;
			std::vector<DirectX::XMFLOAT3> normals;
			std::vector<UINT> indices;

			const unsigned int dataCountVertices() const noexcept {
				return countVertices * sizeof(DirectX::XMFLOAT3);
			}
			const unsigned int dataCountTextureCoordinates() const noexcept {
				return countTextureCoordinates * sizeof(DirectX::XMFLOAT2);
			}
			const unsigned int dataCountNormals() const noexcept {
				return countNormals * sizeof(DirectX::XMFLOAT3);
			}
			const unsigned int dataCountIndices() const noexcept {
				return countIndices;
			}
			const unsigned int dataCount() const noexcept {
				return dataCountVertices() + dataCountTextureCoordinates() + dataCountNormals() + dataCountIndices();
			}
			const unsigned int dataCountBillboard(const bool includeColors) const noexcept {
				return dataCountVertices() + (includeColors ? dataCountVertices() : 0) + dataCountIndices();
			}
		};

	}
}