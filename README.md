# SDEngine
A fully deferred PBR rendering engine complete with stacked post processing and Poisson sampled Variance Shadows. WIP.

## Features
- Module based approach to engine, separating editor out into separate module.
- Dynamic multicast event system for triggering calls across the system (examples being listeneing for input).
- Stacked post processing system including implementations for:
    - Bloom (Gaussian Kernel)
    - Ambient Occlusion
    - Motion Vector based Motion Blur.
    - Reinhard Tone Mapping
- Custom Asset Format
    - Compressed Binary Asset Format to unify storage and access of all asset types (models/textures/etc).
- Actor/Component based world entities.
- Dynamic shader compilation (just press Enter!)
- Signed Distance Field based UI renderer complete with drop shadows, borders, custom border radius, etc.
- UI Rendering System **Pictorum** handles all game UI rendering including:
     - Editable Text Widgets
     - Scroll Containers
     - V/HBoxes
     - Canvas Panels
     - Labels/Solids
     - Images/Textures
     - And anything else you can think of using the widgets system!

# Preprocessor
The preprocessor C# application parses all C++ headers and strips/generates reflection metadata. This is then fed back into the C++ compilation and used when reflecting class members.
