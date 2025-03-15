LEWIS GHRIST: 
- Chunking -
I created a new struct which represents an entire block face keyed by the direction it is pointing. I used this in combination with several unordered maps to construct the vbos. I used a mod system to account for border faces. Rather than updating the model matrix for each face, I kept the world space positions of the current chunk which I used with the chunk space position of each point to directly draw the faces in world space. The main dfficulty was getting the chunk border to work correctly when checkin for empty blocks. 

- Textures - 
Implementing textures was fairly strait forward. I used the shader fun hw as a guide to adding the texture file to a new .qrc system. I then created a large map of uv choordinates (basic blocks implemented in milestone 2, custom textures and procedural asset textures added in milestone 3) keyed on the block type. My system for pasing in UVs was a little strange: I repurposed every part of the color buffer. The first two slots were used to pass in the UVs from my texture map, the third slot was used as a boolean (0 or 1) to represent if the texture is animated or not, and finaly the fourth slot was used for the ambient occlusion value (see bellow). Rendering the water transparency was also simple. I just created two sepperate buffers and functions that processed the vbos. I would used each one based on if the current block was opaque or not. Then during the rendering stage i would render just the opque blocks then the transparent blocks on top.  

- Ambient Oclusion -
The implementation was similar to how I did chunk generation. For each block face, you check the neighboring blocks. The difficulty came with getting the right blocks for each point on the face, due to the fact that they could be in different chunks. In the most extreme case, the three possible occluding blocks could be in three different chunks. The way I got around this was with a somewhat overly extensive check for each position value I would use. I then passed on the number of occluders to the shaders, and used that number to calculate a darkening scalar for the final color.

- Flowers / Grass / Mushrooms - 
For some of the procedurally placed accets, a new form of vbos needed to be generated. Becuase these features are two planes set diagonaly across a block, the old position system wouldn't work. However, I could still reuse some of the value. Because of the order of the point's positions i had previously hardcoded in, there wasn't some neice patter i could take advantage of. Thus the X shape was hard coded in. I added these blocks to the transparend vbos so that the blocks behind them would be correctly rendered. I also slightly adjusted the fragment shader used to render them to discard any fragments with an alpha of 0. This was to solve an issue with fully transparent fragments blocking the opaque fragments behind them when they should have been rendered. This set of blocks was also excluded when taking lighting into account.

- Shadow Mapping -
Shadow mpaing, in theory, is a pretty simple idea. The tough part is the slight adjustments you need to make for it to fit with your geometry well. To make the shadow map, I reused the frame buffer code used to make the post process effects along with a new shader that rendered the fragments z coordinate as the color. Getting the right position for this light camera took a bit of work to get it to both see all the terain but to also smoothly follow the player's movement. With the shadow map texture created, i passed it into final shader to comput which fragments were in shadow. The trouble part was adjusting the bias to remove all atrifacts, which even after hours of adjusting values still resulted in some slight shadow bans when the sun is at a certain angle. Oart of the problem is also with the resolution of the image, although even after upscalling it, still resulted in some minor artifacting. The other issue i had was that the shadows buzzed around alot as the sun moved. A partial fix for this was to clamp part of the cameras MVP values, althoguh that only eliminated jittering along one axis. 



Christina Qiu:
- Water Waves
I had issues with trying to get the blinn phong shader to display correctly on the waves. I found that the issue was because I needed to recalculate the normal by making two adjacent points and getting the cross product of their vectors.

- Post Process Overlay
Overall it was just a lot of tweaking until I got the look I wanted

- Day and Night Cycle
It was hard to get the sun and sky color times correct. For the moon we decided to just have it be the sun but a different color, and turn off shadowmapping during that time period.

- Distance Fog
I originally wanted the fog to make the terrain blend into the sky but instead I decided on making it take the color of the darkest part of the sky to have a pretty silhouette of the mountains.
