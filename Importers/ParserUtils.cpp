#include "pch.h"
#include "ParserUtils.h"

DirectX::XMFLOAT3 Kuplung_DX::Importers::ParserUtils::FixVectorAxis(DirectX::XMFLOAT3 v, int indexForward, int indexUp) {
    //
    //                 +Z
    //                  |
    //                  |           +Y
    //                  |         /
    //                  |       /
    //                  |     /
    //                  |   /
    //                  | /
    //  -X--------------|-----------------+X
    //                 /|
    //               /  |
    //             /    |
    //           /      |
    //         /        |
    //       /          |
    //     -Y           |
    //                 -Z
    //

    DirectX::XMVECTOR vv = DirectX::XMLoadFloat3(&v);

    switch (indexForward) {
        case 0: { // -X Forward
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 0, -90.0, 0));
            break;
        }
        case 1: { // -Y Forward
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 0, 180.0, 0));
            break;
        }
        case 2: { // -Z Forward
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(90.0, 0, 0, 0));
            break;
        }
        case 3: { // X Forward
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 0, 90.0, 0));
            break;
        }
        case 4: { // Y Forward
            break;
        }
        case 5: { // Z Forward
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(-90.0, 0, 0, 0));
            break;
        }
        default:
            break;
    }

    switch (indexUp) {
        case 0: { // -X Up
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, -90.0, 0, 0));
            break;
        }
        case 1: { // -Y Up
            break;
        }
        case 2: { // -Z Up
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 180.0, 0, 0));
            break;
        }
        case 3: { // X Up
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 90.0, 0, 0));
            break;
        }
        case 4: { // Y Up
            vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 180.0, 0, 0));
            break;
        }
        case 5: { // Z Up
//          vv = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&v), DirectX::XMVectorSet(0, 180.0, 0, 0));
            break;
        }
        default:
            break;
    }

    DirectX::XMFLOAT3 v2;
    DirectX::XMStoreFloat3(&v2, vv);
    return v2;
}

