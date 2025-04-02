## Week1

成功运行各项目即可。使用 Visual Studio 双击打开 `.sln` 文件，注意在 Release 模式下运行。

## Week2

1. 实现模型颜色随 z 值由红色渐变为蓝色

   创建 `zzzlib/basic_meshviewer/viewerZcolorDraw.h` 文件，实现 `ViewerZcolorDraw` 类，重点操作为 `glColor3f(-(*fviter)->point()[2] / 2 + 1, 0, (*fviter)->point()[2] / 2 + 1);`，然后在 `main.cpp` 中调用操作。（注意模型的范围在 [-1, 1] 之间）

2. 对模型上的点的法向用小线段进行可视化

   同理，创建 `zzzlib/basic_meshviewer/viewerNormalDraw.h`。重点操作为：

   ```cpp
     glBegin(GL_LINES);
     for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); ++viter)
     {
       M::CVertex *pV = *viter;
       CPoint p = pV->point();
       CPoint n = pV->normal() * 10;
       glVertex3d(p[0], p[1], p[2]);
       glVertex3d(p[0] + n[0], p[1] + n[1], p[2] + n[2]);
     }
   ```

   `pV->normal() * 10` 用于放大法向线段长度。

3. 计算封闭模型的体积

   在 `main.cpp` 中添加 `double calculateVolume(CTMesh &mesh)` 方法，体积计算公式：

   $$
   V = \sum_{Face} \frac{p_1 \cdot (p_2 \times p_3)}6
   $$

   其中 p1, p2, p3 为三角形的三个顶点。

## Week3

运行平均值公式进行模型变形：输入人脸，固定边界，对内部点取加权平均，迭代至收敛，保存为新文件，并可视化；设置不同的权值，对结果进行对比。
