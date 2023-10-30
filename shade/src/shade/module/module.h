#pragma once

namespace Shade {

    class Module {
    public:
        Module();
        ~Module();

        void Update(float DeltaSeconds);
        void Render();
    };

}