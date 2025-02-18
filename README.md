# OpenGL_L
Advancing from beginner to mastery in OpenGL while developing an efficient framework to systematically organize study materials and code, enabling streamlined learning and practical experimentation.

𝐖𝐞𝐢𝐠𝐡𝐭𝐞𝐝-𝐁𝐥𝐞𝐧𝐝𝐞𝐝 𝐎𝐫𝐝𝐞𝐫 𝐈𝐧𝐝𝐞𝐩𝐞𝐧𝐝𝐞𝐧𝐭 𝐓𝐫𝐚𝐧𝐬𝐩𝐚𝐫𝐞𝐧𝐜𝐲 (𝐎𝐈𝐓)
![image](https://github.com/user-attachments/assets/fe162d80-ee96-4fae-9486-342d0c0c1e6d)
This algorithm strikes an impressive balance between computational efficiency and visual quality, making it a popular choice for real-time rendering in games and simulations.

Unlike traditional OIT methods like depth peeling, 𝐖𝐞𝐢𝐠𝐡𝐭𝐞𝐝 𝐁𝐥𝐞𝐧𝐝𝐞𝐝 𝐎𝐈𝐓 avoids complex sorting and achieves smooth, visually pleasing transparency in a single render pass.

----
#### Trilinear Filtering vs Anisotropic Filtering

**Trilinear filtering** interpolates between mipmap levels but assumes a uniform view, leading to blurring and noticeable detail loss at oblique angles due to aggressive mipmap blending.
![trilinear filtering](https://github.com/user-attachments/assets/55baa975-56ec-48ec-b62b-01948fad9bfc)

**Anisotropic filtering** improves upon trilinear filtering by sampling textures non-uniformly, taking more samples along the direction of distortion. This reduces blurriness at shallow viewing angles, preserving more detail on large terrains extending toward the horizon.
![anisotropic filtering](https://github.com/user-attachments/assets/057aa2f1-8bbe-4174-8b0f-45d3aa1a2f93)


For large terrains, anisotropic filtering significantly enhances visual clarity, particularly at oblique viewing angles, making it a preferred choice over trilinear filtering while maintaining a reasonable computational cost.
