# OpenGL_L
Advancing from beginner to mastery in OpenGL while developing an efficient framework to systematically organize study materials and code, enabling streamlined learning and practical experimentation.

----
### Weighted-Blended Order Independent Transparency (OIT)
![image](https://github.com/user-attachments/assets/fe162d80-ee96-4fae-9486-342d0c0c1e6d)
This algorithm strikes an impressive balance between computational efficiency and visual quality, making it a popular choice for real-time rendering in games and simulations.

Unlike traditional OIT methods like depth peeling, **Weighted Blended OIT** avoids complex sorting and achieves smooth, visually pleasing transparency in a single render pass.

----
### Trilinear Filtering vs Anisotropic Filtering

**Trilinear filtering** interpolates between mipmap levels but assumes a uniform view, leading to blurring and noticeable detail loss at oblique angles due to aggressive mipmap blending.
![trilinear filtering](https://github.com/user-attachments/assets/55baa975-56ec-48ec-b62b-01948fad9bfc)

**Anisotropic filtering** improves upon trilinear filtering by sampling textures non-uniformly, taking more samples along the direction of distortion. This reduces blurriness at shallow viewing angles, preserving more detail on large terrains extending toward the horizon.
![anisotropic filtering](https://github.com/user-attachments/assets/057aa2f1-8bbe-4174-8b0f-45d3aa1a2f93)


For large terrains, anisotropic filtering significantly enhances visual clarity, particularly at oblique viewing angles, making it a preferred choice over trilinear filtering while maintaining a reasonable computational cost.


----
### Rugged Terrain 
![image](https://github.com/user-attachments/assets/1faf8c2c-2e94-4bf0-93eb-e44e31364ebf)

