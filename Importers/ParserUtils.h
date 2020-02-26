#pragma once

namespace Kuplung_DX
{
    namespace Importers
    {
        class ParserUtils {
        public:
            static DirectX::XMFLOAT3 FixVectorAxis(DirectX::XMFLOAT3 v, int indexForward, int indexUp);
        };

    }
}