#pragma once

#include <vector>
#include <Utilities\CXXUtils.h>

using namespace DirectX;

namespace Kuplung_DX
{
	namespace Models
	{
		using namespace DirectX;
		using namespace Kuplung_DX;

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
			std::vector<XMFLOAT3> vertices;
			std::vector<XMFLOAT2> texture_coordinates;
			std::vector<XMFLOAT3> normals;
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

		static void Kuplung_printObjModels(const std::vector<MeshModel>& models, bool byIndices) {
			std::string output("");
			for (size_t i = 0; i < models.size(); i++) {
				MeshModel m = models[i];
				output += Utilities::CXXUtils::StringFormat("model.ID = %i\r\r\n", m.ID);
				output += Utilities::CXXUtils::StringFormat("model.countIndices = %i (%i)\r\r\n", m.countIndices, ((*std::max_element(m.indices.begin(), m.indices.end())) + 1));
				output += Utilities::CXXUtils::StringFormat("model.countNormals = %i (%i)\r\n", m.countNormals, (m.countNormals * 3));
				output += Utilities::CXXUtils::StringFormat("model.countTextureCoordinates = %i (%i)\r\n", m.countTextureCoordinates, (m.countTextureCoordinates * 2));
				output += Utilities::CXXUtils::StringFormat("model.countVertices = %i (%i)\r\n", m.countVertices, (m.countVertices * 3));
				output += Utilities::CXXUtils::StringFormat("model.MaterialTitle = %s\r\n", m.MaterialTitle.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelTitle = %s\r\n", m.ModelTitle.c_str());

				if (byIndices) {
					output += "m.geometry :\r\n";
					for (size_t j = 0; j < m.indices.size(); j++) {
						const size_t idx = m.indices[j];
						std::string geom = Utilities::CXXUtils::StringFormat("index = %i ---> ", idx);
						const XMFLOAT3 vert = m.vertices[idx];
						const XMFLOAT2 tc = m.texture_coordinates[idx];
						const XMFLOAT3 n = m.normals[idx];
						geom += Utilities::CXXUtils::StringFormat("vertex = [%g, %g, %g]", vert.x, vert.y, vert.z);
						geom += Utilities::CXXUtils::StringFormat(", uv = [%g, %g]", tc.x, tc.y);
						geom += Utilities::CXXUtils::StringFormat(", normal = [%g, %g, %g]", n.x, n.y, n.z);
						//printf("%s\r\n", geom.c_str());
					}
				}
				else {
					std::string verts;
					for (size_t j = 0; j < m.vertices.size(); j++) {
						const XMFLOAT3 v = m.vertices[j];
						verts += Utilities::CXXUtils::StringFormat("[%g, %g, %g], ", v.x, v.y, v.z);
					}
					printf("m.vertices : %s\r\n", verts.c_str());

					std::string uvs;
					for (size_t j = 0; j < m.texture_coordinates.size(); j++) {
						uvs += Utilities::CXXUtils::StringFormat("[%g, %g], ", m.texture_coordinates[j].x, m.texture_coordinates[j].y);
					}
					printf("m.texture_coordinates : %s\r\n", uvs.c_str());

					std::string normals;
					for (size_t j = 0; j < m.normals.size(); j++) {
						const XMFLOAT3 n = m.normals[j];
						normals += Utilities::CXXUtils::StringFormat("[%f, %f, %f], ", n.x, n.y, n.z);
					}
					printf("m.normals : %s\r\n", normals.c_str());

					std::string indices;
					for (size_t j = 0; j < m.indices.size(); j++) {
						indices += Utilities::CXXUtils::StringFormat("%i, ", m.indices[j]);
					}
					output += Utilities::CXXUtils::StringFormat("m.indices : %s\r\n", indices.c_str());
				}

				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.MaterialID = %i\r\n", m.ModelMaterial.MaterialID);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.MaterialTitle = %s\r\n", m.ModelMaterial.MaterialTitle.c_str());

				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.AmbientColor = %g, %g, %g\r\n", m.ModelMaterial.AmbientColor.x, m.ModelMaterial.AmbientColor.y, m.ModelMaterial.AmbientColor.z);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.DiffuseColor = %g, %g, %g\r\n", m.ModelMaterial.DiffuseColor.x, m.ModelMaterial.DiffuseColor.y, m.ModelMaterial.DiffuseColor.z);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.SpecularColor = %g, %g, %g\r\n", m.ModelMaterial.SpecularColor.x, m.ModelMaterial.SpecularColor.y, m.ModelMaterial.SpecularColor.z);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.EmissionColor = %g, %g, %g\r\n", m.ModelMaterial.EmissionColor.x, m.ModelMaterial.EmissionColor.y, m.ModelMaterial.EmissionColor.z);

				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.SpecularExp = %g\r\n", m.ModelMaterial.SpecularExp);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.Transparency = %g\r\n", m.ModelMaterial.Transparency);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.OpticalDensity = %g\r\n", m.ModelMaterial.OpticalDensity);
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.IlluminationMode = %u\r\n", m.ModelMaterial.IlluminationMode);

				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_ambient.Filename = %s\r\n", m.ModelMaterial.TextureAmbient.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_diffuse.Filename = %s\r\n", m.ModelMaterial.TextureDiffuse.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_specular.Filename = %s\r\n", m.ModelMaterial.TextureSpecular.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_specularExp.Filename = %s\r\n", m.ModelMaterial.TextureSpecularExp.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_dissolve.Filename = %s\r\n", m.ModelMaterial.TextureDissolve.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_bump.Filename = %s\r\n", m.ModelMaterial.TextureBump.Filename.c_str());
				output += Utilities::CXXUtils::StringFormat("model.ModelMaterial.textures_displacement.Filename = %s\r\n", m.ModelMaterial.TextureDisplacement.Filename.c_str());
			}
			output += "\r\n--------\r\n";
			auto formattedText = Utilities::CXXUtils::StringToWString(output).append(L"\r\n");
			OutputDebugString(formattedText.c_str());
		}
	}
}