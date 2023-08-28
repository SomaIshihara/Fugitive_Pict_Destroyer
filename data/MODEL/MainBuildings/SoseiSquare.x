xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 389;
 89.78620;135.17160;-48.61290;,
 100.00000;135.17160;-48.61290;,
 100.00000;29.26590;-48.61290;,
 89.78620;11.40110;-48.61290;,
 89.78620;135.17160;-100.00000;,
 89.78620;135.17160;-61.34730;,
 89.78620;11.40110;-61.34730;,
 89.78620;11.40110;-100.00000;,
 -12.52580;135.17160;-61.34730;,
 -12.52580;135.17160;-100.00000;,
 -12.52580;62.26470;-100.00000;,
 -12.52580;62.26470;-61.34730;,
 -22.04420;62.26470;-48.61290;,
 -22.04420;62.26470;100.00000;,
 -12.52580;62.26470;100.00000;,
 -12.52580;62.26470;-61.34730;,
 -12.52580;135.17160;100.00000;,
 -12.52580;135.17160;-61.34730;,
 89.78620;135.17160;-48.61290;,
 43.74010;135.17160;100.00000;,
 100.00000;135.17160;100.00000;,
 100.00000;135.17160;-48.61290;,
 100.00000;11.40110;100.00000;,
 100.00000;29.26590;-48.61290;,
 -22.04420;62.26470;-48.61290;,
 -22.04420;66.43020;-61.34730;,
 -22.04420;75.58860;-61.34730;,
 -22.20690;75.58860;-48.08920;,
 100.00000;11.40110;100.00000;,
 100.00000;-0.01890;100.00000;,
 100.00000;-0.01890;34.09870;,
 100.00000;11.40110;34.09870;,
 89.78620;11.40110;-100.00000;,
 89.78620;11.40110;-61.34730;,
 89.78620;-0.01890;-61.34730;,
 89.78620;-0.01890;-100.00000;,
 89.78620;11.40110;-48.61290;,
 89.78620;-0.01890;-48.61290;,
 -100.00000;46.02420;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -60.89270;11.40110;-61.34730;,
 -91.43120;11.40110;-61.34730;,
 -60.89270;62.26470;-100.00000;,
 -60.89270;11.40110;-100.00000;,
 -91.43120;11.40110;-53.17550;,
 -91.43120;-0.01890;-53.17550;,
 -91.43120;-0.01890;82.98720;,
 -91.43120;11.40110;82.98720;,
 -100.00000;71.22820;82.98720;,
 -100.00000;37.94150;82.98720;,
 -100.00000;29.26590;100.00000;,
 -100.00000;92.53810;100.00000;,
 -60.89270;11.40110;100.00000;,
 -22.04420;92.53810;100.00000;,
 -100.00000;62.26470;-20.87290;,
 -100.00000;66.43020;-20.87290;,
 -100.00000;66.43020;-61.34730;,
 -100.00000;62.26470;-61.34730;,
 -100.00000;52.64790;-61.34730;,
 -100.00000;52.64790;-34.74290;,
 -100.00000;57.54220;-34.74290;,
 -100.00000;57.54220;-61.34730;,
 -111.20050;29.26590;52.52250;,
 -111.20050;29.26590;100.00000;,
 -100.00000;29.26590;100.00000;,
 -100.00000;29.26590;52.52250;,
 -111.20050;11.40110;-61.34730;,
 -111.20050;11.40110;100.00000;,
 -111.20050;66.43020;-61.34730;,
 -111.20050;71.22820;-61.34730;,
 -100.00000;71.22820;-61.34730;,
 -100.00000;66.43020;-61.34730;,
 -111.20050;66.43020;82.98720;,
 -111.20050;46.02420;82.98720;,
 -111.20050;62.26470;-20.87290;,
 -111.20050;66.43020;-20.87290;,
 -100.00000;66.43020;-20.87290;,
 -100.00000;62.26470;-20.87290;,
 -111.20050;37.94150;52.52250;,
 -111.20050;29.26590;52.52250;,
 -100.00000;29.26590;52.52250;,
 -100.00000;37.94150;52.52250;,
 -111.20050;57.54220;-61.34730;,
 -111.20050;62.26470;-61.34730;,
 -100.00000;62.26470;-61.34730;,
 -100.00000;57.54220;-61.34730;,
 -111.20050;62.26470;-61.34730;,
 -111.20050;62.26470;-20.87290;,
 -100.00000;62.26470;-20.87290;,
 -100.00000;62.26470;-61.34730;,
 -111.20050;57.54220;-34.74290;,
 -111.20050;57.54220;-61.34730;,
 -111.20050;52.64790;-34.74290;,
 -111.20050;37.94150;82.98720;,
 -100.00000;37.94150;82.98720;,
 -111.20050;50.54770;-61.34730;,
 -100.00000;50.54770;-61.34730;,
 -100.00000;52.64790;-61.34730;,
 -111.20050;52.64790;-61.34730;,
 -111.20050;50.54770;-34.74290;,
 -111.20050;50.54770;-61.34730;,
 -111.20050;52.64790;-61.34730;,
 -100.00000;50.54770;-34.74290;,
 -100.00000;50.54770;-61.34730;,
 -111.20050;66.43020;-9.90100;,
 -111.20050;49.26260;-9.90100;,
 -111.20050;49.26260;-34.74290;,
 -111.20050;49.26260;-61.34730;,
 -111.20050;49.26260;-61.34730;,
 -100.00000;49.26260;-61.34730;,
 -111.20050;46.02420;-61.34730;,
 -111.20050;11.40110;-61.34730;,
 -111.20050;37.94150;-9.90100;,
 -111.20050;46.02420;-9.90100;,
 -111.20050;49.26260;-9.90100;,
 -111.20050;49.26260;-34.74290;,
 -111.20050;46.02420;-61.34730;,
 -100.00000;46.02420;-61.34730;,
 -22.20690;92.53810;83.51080;,
 -22.04420;92.53810;100.00000;,
 -100.16270;92.53810;83.51080;,
 -41.63120;92.53810;83.51080;,
 -41.63120;299.49951;83.51080;,
 -100.16270;299.49951;83.51080;,
 -22.20690;299.49951;-48.08920;,
 -22.20690;299.49951;83.51080;,
 -22.04420;62.26470;-48.61290;,
 -41.63120;299.49951;-48.08920;,
 -22.20690;299.49951;-48.08920;,
 -41.63120;75.58860;-48.08920;,
 -100.16270;75.58860;-48.08920;,
 -100.16270;299.49951;-48.08920;,
 -22.04420;75.58860;-61.34730;,
 -100.00000;75.58860;-61.34730;,
 -22.04420;66.43020;-61.34730;,
 -100.00000;66.43020;-61.34730;,
 -41.63120;299.49951;83.51080;,
 -41.63120;92.53810;83.51080;,
 -22.04420;62.26470;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -12.52580;62.26470;-100.00000;,
 -60.89270;62.26470;-100.00000;,
 -12.52580;62.26470;-100.00000;,
 -52.75890;11.40110;-100.00000;,
 -60.89270;11.40110;100.00000;,
 -60.89270;-0.01890;100.00000;,
 1.72280;-0.01890;100.00000;,
 1.72280;11.40110;100.00000;,
 -60.89270;10.76250;82.98720;,
 -91.43120;11.40110;82.98720;,
 -91.43120;-0.01890;82.98720;,
 -60.89270;-0.01890;82.98720;,
 -60.89270;11.40110;100.00000;,
 -60.89270;-0.01890;100.00000;,
 -111.20050;11.40110;100.00000;,
 -60.89270;11.40110;-53.17550;,
 -91.43120;11.40110;-53.17550;,
 -60.89270;-0.01890;-53.17550;,
 -91.43120;-0.01890;-53.17550;,
 -52.75890;11.40110;-100.00000;,
 -52.75890;11.40110;-61.34730;,
 -52.75890;-0.01890;-100.00000;,
 -52.75890;-0.01890;-61.34730;,
 -52.75890;11.40110;-53.17550;,
 -52.75890;-0.01890;-53.17550;,
 89.67370;11.40110;-34.75620;,
 89.78620;11.40110;-48.61290;,
 100.00000;11.40110;-48.61290;,
 99.70010;11.48650;-34.88670;,
 89.78620;-0.01890;-34.74290;,
 100.00000;-0.01890;-34.74290;,
 86.83040;11.38650;-48.62490;,
 86.83040;-0.03350;-48.62490;,
 89.78620;-0.01890;-48.61290;,
 86.71800;11.38650;-34.76820;,
 86.83040;-0.03350;-34.75490;,
 100.00000;11.40110;19.50610;,
 100.00000;-0.01890;19.50610;,
 100.00000;-0.01890;-34.74290;,
 99.70010;11.48650;-34.88670;,
 100.00000;11.40110;34.09870;,
 100.00000;-0.01890;34.09870;,
 -111.20050;29.26590;-9.90100;,
 -111.20050;37.94150;-9.90100;,
 -111.20050;46.02420;82.98720;,
 -111.20050;46.02420;-9.90100;,
 -111.20050;49.26260;-9.90100;,
 -111.20050;46.02420;82.98720;,
 -111.20050;66.43020;82.98720;,
 -111.20050;66.43020;-9.90100;,
 -111.20050;71.22820;82.98720;,
 -111.20050;71.22820;-61.34730;,
 -100.00000;71.22820;82.98720;,
 -100.00000;71.22820;-61.34730;,
 -100.16270;75.58860;83.17440;,
 -100.16270;75.58860;-48.08920;,
 -41.63120;299.49951;-48.08920;,
 -100.16270;299.49951;-48.08920;,
 -100.16270;299.49951;83.51080;,
 -41.63120;299.49951;83.51080;,
 58.17390;11.40110;100.00000;,
 58.17390;-0.01890;100.00000;,
 100.00000;-0.01890;100.00000;,
 100.00000;11.40110;100.00000;,
 43.74010;11.40110;100.00000;,
 43.74010;-0.01890;100.00000;,
 17.09070;11.40110;100.00000;,
 1.72280;11.40110;100.00000;,
 1.72280;-0.01890;100.00000;,
 17.09070;-0.01890;100.00000;,
 17.09070;11.40110;100.00000;,
 43.74010;11.40110;100.00000;,
 43.74010;135.17160;100.00000;,
 -12.52580;135.17160;100.00000;,
 89.78620;135.17160;-61.34730;,
 -12.52580;135.17160;-61.34730;,
 -52.75890;-0.01890;-100.00000;,
 -52.75890;11.40110;-100.00000;,
 89.78620;11.40110;-100.00000;,
 89.78620;-0.01890;-100.00000;,
 -12.52580;135.17160;-61.34730;,
 89.78620;135.17160;-61.34730;,
 89.78620;135.17160;-100.00000;,
 -12.52580;135.17160;-100.00000;,
 89.78620;11.40110;-100.00000;,
 -12.52580;62.26470;-100.00000;,
 -22.04420;62.26470;-61.34730;,
 -22.04420;92.53810;100.00000;,
 -60.89270;11.40110;100.00000;,
 -22.20690;299.49951;-48.08920;,
 -22.04420;62.26470;-48.61290;,
 100.00000;29.26590;-48.61290;,
 100.00000;29.26590;-48.61290;,
 100.00000;29.26590;-48.61290;,
 100.00000;11.40110;100.00000;,
 100.00000;29.26590;-48.61290;,
 89.78620;11.40110;-48.61290;,
 100.00000;29.26590;-48.61290;,
 -12.52580;62.26470;-61.34730;,
 43.74010;135.17160;100.00000;,
 58.17390;11.40110;100.00000;,
 100.00000;11.40110;100.00000;,
 89.78620;135.17160;-61.34730;,
 -12.52580;135.17160;100.00000;,
 1.72280;11.40110;100.00000;,
 -12.52580;135.17160;100.00000;,
 89.78620;11.40110;-100.00000;,
 -12.52580;62.26470;-100.00000;,
 -12.52580;62.26470;-100.00000;,
 -22.04420;75.58860;-61.34730;,
 -100.00000;66.43020;-61.34730;,
 -100.00000;62.26470;-61.34730;,
 -100.00000;75.58860;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -100.00000;46.02420;-61.34730;,
 -100.00000;49.26260;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -100.00000;52.64790;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -100.00000;57.54220;-61.34730;,
 -60.89270;62.26470;-61.34730;,
 -100.00000;46.02420;-61.34730;,
 -111.20050;11.40110;-61.34730;,
 -111.20050;11.40110;-61.34730;,
 -100.00000;46.02420;-61.34730;,
 -91.43120;11.40110;-61.34730;,
 -111.20050;11.40110;-61.34730;,
 -111.20050;29.26590;-9.90100;,
 -111.20050;11.40110;-61.34730;,
 -91.43120;11.40110;-53.17550;,
 -100.00000;71.22820;-61.34730;,
 -100.16270;75.58860;-48.08920;,
 -100.00000;75.58860;-61.34730;,
 -111.20050;71.22820;-61.34730;,
 -111.20050;71.22820;-61.34730;,
 -111.20050;66.43020;-61.34730;,
 -111.20050;66.43020;-61.34730;,
 -111.20050;11.40110;-61.34730;,
 -111.20050;46.02420;-61.34730;,
 -111.20050;46.02420;82.98720;,
 -111.20050;11.40110;-61.34730;,
 -100.00000;29.26590;52.52250;,
 -111.20050;49.26260;-9.90100;,
 -111.20050;52.64790;-34.74290;,
 -111.20050;49.26260;-9.90100;,
 -111.20050;62.26470;-20.87290;,
 -111.20050;57.54220;-34.74290;,
 -111.20050;49.26260;-9.90100;,
 -100.00000;66.43020;-20.87290;,
 -111.20050;46.02420;82.98720;,
 -111.20050;49.26260;-9.90100;,
 -100.00000;37.94150;82.98720;,
 -111.20050;46.02420;82.98720;,
 -111.20050;71.22820;82.98720;,
 -100.16270;75.58860;83.17440;,
 -100.00000;29.26590;100.00000;,
 -100.16270;299.49951;83.51080;,
 -60.89270;11.40110;100.00000;,
 1.72280;11.40110;100.00000;,
 -22.04420;62.26470;100.00000;,
 -22.04420;92.53810;100.00000;,
 100.00000;11.40110;100.00000;,
 43.74010;135.17160;100.00000;,
 1.72280;11.40110;100.00000;,
 -12.52580;62.26470;100.00000;,
 -60.89270;11.40110;100.00000;,
 -100.00000;29.26590;100.00000;,
 -111.20050;11.40110;100.00000;,
 -41.63120;299.49951;-48.08920;,
 -41.63120;299.49951;83.51080;,
 -108.07290;57.33620;-55.05250;,
 -106.99630;57.11720;-56.18420;,
 -108.07290;57.11720;-56.65300;,
 -108.07290;57.33620;-55.05250;,
 -106.55030;57.11720;-55.05250;,
 -108.07290;57.33620;-55.05250;,
 -106.99630;57.11720;-53.92090;,
 -108.07290;57.33620;-55.05250;,
 -108.07290;57.11720;-53.45210;,
 -108.07290;57.33620;-55.05250;,
 -109.14950;57.11720;-53.92090;,
 -108.07290;57.33620;-55.05250;,
 -109.59550;57.11720;-55.05250;,
 -108.07290;57.33620;-55.05250;,
 -109.14950;57.11720;-56.18420;,
 -108.07290;57.33620;-55.05250;,
 -108.07290;57.11720;-56.65300;,
 -106.08360;56.49350;-57.14360;,
 -108.07290;56.49350;-58.00970;,
 -105.25970;56.49350;-55.05250;,
 -106.08360;56.49350;-52.96150;,
 -108.07290;56.49350;-52.09530;,
 -110.06220;56.49350;-52.96150;,
 -110.88610;56.49350;-55.05250;,
 -110.06220;56.49350;-57.14360;,
 -108.07290;56.49350;-58.00970;,
 -105.47370;55.56020;-57.78460;,
 -108.07290;55.56020;-58.91620;,
 -104.39710;55.56020;-55.05250;,
 -105.47370;55.56020;-52.32050;,
 -108.07290;55.56020;-51.18880;,
 -110.67200;55.56020;-52.32050;,
 -111.74860;55.56020;-55.05250;,
 -110.67200;55.56020;-57.78460;,
 -108.07290;55.56020;-58.91620;,
 -105.25970;54.45920;-58.00970;,
 -108.07290;54.45920;-59.23460;,
 -104.09430;54.45920;-55.05250;,
 -105.25970;54.45920;-52.09530;,
 -108.07290;54.45920;-50.87040;,
 -110.88610;54.45920;-52.09530;,
 -112.05150;54.45920;-55.05250;,
 -110.88610;54.45920;-58.00970;,
 -108.07290;54.45920;-59.23460;,
 -105.47370;53.35820;-57.78460;,
 -108.07290;53.35820;-58.91620;,
 -104.39710;53.35820;-55.05250;,
 -105.47370;53.35820;-52.32050;,
 -108.07290;53.35820;-51.18880;,
 -110.67200;53.35820;-52.32050;,
 -111.74860;53.35820;-55.05250;,
 -110.67200;53.35820;-57.78460;,
 -108.07290;53.35820;-58.91620;,
 -106.08360;52.42490;-57.14360;,
 -108.07290;52.42490;-58.00970;,
 -105.25970;52.42490;-55.05250;,
 -106.08360;52.42490;-52.96150;,
 -108.07290;52.42490;-52.09530;,
 -110.06220;52.42490;-52.96150;,
 -110.88610;52.42490;-55.05250;,
 -110.06220;52.42490;-57.14360;,
 -108.07290;52.42490;-58.00970;,
 -106.99630;51.80120;-56.18420;,
 -108.07290;51.80120;-56.65300;,
 -106.55030;51.80120;-55.05250;,
 -106.99630;51.80120;-53.92090;,
 -108.07290;51.80120;-53.45210;,
 -109.14950;51.80120;-53.92090;,
 -109.59550;51.80120;-55.05250;,
 -109.14950;51.80120;-56.18420;,
 -108.07290;51.80120;-56.65300;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;,
 -108.07290;51.58220;-55.05250;;
 
 209;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;5,0,3,6;,
 4;12,13,14,15;,
 4;15,14,16,17;,
 4;18,19,20,21;,
 4;21,20,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,34,33;,
 4;38,39,40,41;,
 4;39,42,43,40;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;51,50,52,53;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;63,62,66,67;,
 4;68,69,70,71;,
 4;72,73,49,48;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;90,91,61,60;,
 4;87,86,91,90;,
 4;92,90,60,59;,
 4;93,78,81,94;,
 4;95,96,97,98;,
 4;99,100,101,92;,
 4;102,99,92,59;,
 4;103,102,59,58;,
 4;100,99,102,103;,
 4;104,75,74,105;,
 4;106,92,101,107;,
 4;108,98,97,109;,
 4;110,111,112,113;,
 4;110,113,114,115;,
 4;116,108,109,117;,
 4;118,119,13,12;,
 4;120,51,53,121;,
 4;122,123,120,121;,
 4;124,125,118,126;,
 4;127,128,27,129;,
 4;127,129,130,131;,
 4;129,132,133,130;,
 4;132,134,135,133;,
 4;136,137,118,125;,
 4;134,138,139,135;,
 4;138,140,141,139;,
 4;142,143,43,42;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;152,148,151,153;,
 4;149,148,152,154;,
 4;155,156,41,40;,
 4;156,155,157,158;,
 4;159,160,40,43;,
 4;160,159,161,162;,
 4;163,164,157,155;,
 4;160,163,155,40;,
 4;164,163,160,162;,
 4;165,166,167,168;,
 4;169,165,168,170;,
 4;171,172,173,166;,
 4;174,171,166,165;,
 4;175,174,165,169;,
 4;175,172,171,174;,
 4;176,177,178,179;,
 4;180,181,177,176;,
 4;182,79,78,183;,
 4;183,78,184,185;,
 4;186,187,188,189;,
 4;189,188,190,191;,
 4;191,190,192,193;,
 4;193,192,194,195;,
 4;196,197,198,199;,
 4;195,194,198,197;,
 4;200,201,202,203;,
 4;204,205,201,200;,
 4;206,207,208,209;,
 4;206,209,205,204;,
 4;210,211,212,213;,
 4;213,212,214,215;,
 4;216,217,218,219;,
 4;220,221,222,223;,
 4;223,222,224,225;,
 3;24,226,25;,
 3;13,227,228;,
 3;229,230,27;,
 3;231,180,176;,
 3;232,176,179;,
 3;233,168,167;,
 3;234,31,235;,
 3;236,237,167;,
 3;238,226,24;,
 3;239,240,241;,
 3;239,18,242;,
 3;243,244,210;,
 3;245,14,244;,
 3;246,217,247;,
 3;248,226,11;,
 3;249,129,27;,
 3;250,139,251;,
 3;252,135,70;,
 3;253,254,255;,
 3;256,255,257;,
 3;258,257,259;,
 3;260,259,251;,
 3;261,262,116;,
 3;263,264,265;,
 3;266,267,112;,
 3;268,265,269;,
 3;270,271,272;,
 3;273,75,104;,
 3;274,275,75;,
 3;276,56,55;,
 3;277,79,182;,
 3;278,106,107;,
 3;279,78,93;,
 3;280,44,47;,
 3;281,94,81;,
 3;282,283,115;,
 3;284,285,286;,
 3;287,286,283;,
 3;288,75,275;,
 3;289,290,185;,
 3;291,292,93;,
 3;48,293,72;,
 3;51,294,48;,
 3;51,120,294;,
 3;295,94,65;,
 3;296,194,120;,
 3;297,298,299;,
 3;300,118,137;,
 3;301,20,19;,
 3;302,211,240;,
 3;303,304,299;,
 3;305,306,307;,
 4;125,124,308,309;,
 3;67,66,47;,
 3;67,64,63;,
 3;310,311,312;,
 3;313,314,311;,
 3;315,316,314;,
 3;317,318,316;,
 3;319,320,318;,
 3;321,322,320;,
 3;323,324,322;,
 3;325,326,324;,
 4;312,311,327,328;,
 4;311,314,329,327;,
 4;314,316,330,329;,
 4;316,318,331,330;,
 4;318,320,332,331;,
 4;320,322,333,332;,
 4;322,324,334,333;,
 4;324,326,335,334;,
 4;328,327,336,337;,
 4;327,329,338,336;,
 4;329,330,339,338;,
 4;330,331,340,339;,
 4;331,332,341,340;,
 4;332,333,342,341;,
 4;333,334,343,342;,
 4;334,335,344,343;,
 4;337,336,345,346;,
 4;336,338,347,345;,
 4;338,339,348,347;,
 4;339,340,349,348;,
 4;340,341,350,349;,
 4;341,342,351,350;,
 4;342,343,352,351;,
 4;343,344,353,352;,
 4;346,345,354,355;,
 4;345,347,356,354;,
 4;347,348,357,356;,
 4;348,349,358,357;,
 4;349,350,359,358;,
 4;350,351,360,359;,
 4;351,352,361,360;,
 4;352,353,362,361;,
 4;355,354,363,364;,
 4;354,356,365,363;,
 4;356,357,366,365;,
 4;357,358,367,366;,
 4;358,359,368,367;,
 4;359,360,369,368;,
 4;360,361,370,369;,
 4;361,362,371,370;,
 4;364,363,372,373;,
 4;363,365,374,372;,
 4;365,366,375,374;,
 4;366,367,376,375;,
 4;367,368,377,376;,
 4;368,369,378,377;,
 4;369,370,379,378;,
 4;370,371,380,379;,
 3;373,372,381;,
 3;372,374,382;,
 3;374,375,383;,
 3;375,376,384;,
 3;376,377,385;,
 3;377,378,386;,
 3;378,379,387;,
 3;379,380,388;;
 
 MeshMaterialList {
  5;
  209;
  2,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  4,
  3,
  2,
  2,
  1,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  2,
  1,
  2,
  1,
  4,
  2,
  4,
  2,
  2,
  3,
  2,
  3,
  3,
  2,
  3,
  2,
  2,
  2,
  2,
  2,
  3,
  4,
  3,
  1,
  2,
  1,
  2,
  2,
  2,
  2,
  1,
  4,
  3,
  3,
  4,
  2,
  2,
  3,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.721600;0.176000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.307200;0.586400;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.743200;0.743200;0.743200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.460800;0.460800;0.460800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.656000;0.656000;0.656000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  98;
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.004259;-0.999986;0.003145;,
  -0.999538;-0.025558;0.016439;,
  -0.999718;-0.013189;0.019728;,
  0.000000;-1.000000;-0.000000;,
  -0.006502;-0.006895;-0.999955;,
  0.999949;0.006028;0.008107;,
  0.999996;0.001514;-0.002198;,
  -0.999812;-0.015373;0.011836;,
  1.000000;0.000342;0.000537;,
  0.999987;0.003014;0.004054;,
  -0.001218;-0.004010;-0.999991;,
  0.004599;-0.999988;0.001593;,
  -0.004060;0.000000;0.999992;,
  0.004066;-0.001124;-0.999991;,
  0.004939;-0.999988;0.000040;,
  0.999996;-0.002038;-0.002070;,
  0.999978;0.006498;-0.001368;,
  0.999986;-0.004229;0.003260;,
  0.999997;0.002264;0.000928;,
  0.999984;0.002687;-0.004933;,
  -0.003483;-0.999974;0.006252;,
  -0.010448;-0.999769;0.018755;,
  -0.005225;-0.999942;0.009378;,
  0.999980;0.004921;0.004056;,
  0.999912;0.012995;-0.002736;,
  0.999986;-0.002717;0.004522;,
  -0.999807;-0.018647;-0.006136;,
  -0.999914;-0.012431;-0.004091;,
  -0.954976;0.000000;-0.296684;,
  0.999761;0.000000;0.021843;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;0.000000;,
  -0.999925;0.000000;-0.012271;,
  -0.999988;-0.000098;0.004933;,
  0.000000;0.000000;1.000000;,
  -0.000000;1.000000;0.000003;,
  0.000000;0.960401;-0.278622;,
  0.206267;0.958524;-0.196687;,
  0.291217;0.956657;0.000005;,
  0.206271;0.958522;0.196691;,
  0.000000;0.960402;0.278619;,
  -0.206270;0.958522;0.196690;,
  -0.291217;0.956657;0.000005;,
  -0.206266;0.958524;-0.196687;,
  -0.000002;0.818156;-0.574996;,
  0.422049;0.811553;-0.404049;,
  0.593199;0.805056;0.000006;,
  0.422057;0.811550;0.404048;,
  -0.000002;0.818163;0.574987;,
  -0.422069;0.811541;0.404054;,
  -0.593219;0.805041;0.000006;,
  -0.422061;0.811544;-0.404055;,
  -0.000003;0.505885;-0.862601;,
  0.625269;0.497081;-0.601622;,
  0.872501;0.488612;0.000004;,
  0.625271;0.497090;0.601613;,
  -0.000004;0.505906;0.862589;,
  -0.625271;0.497097;0.601607;,
  -0.872494;0.488625;0.000004;,
  -0.625270;0.497087;-0.601616;,
  -0.000003;0.000000;-1.000000;,
  0.719784;-0.000000;-0.694198;,
  1.000000;-0.000000;0.000002;,
  0.719788;0.000000;0.694193;,
  -0.000003;0.000001;1.000000;,
  -0.719789;0.000001;0.694193;,
  -1.000000;0.000000;0.000002;,
  -0.719784;0.000000;-0.694198;,
  -0.000003;-0.505884;-0.862601;,
  0.625269;-0.497081;-0.601622;,
  0.872501;-0.488612;0.000004;,
  0.625271;-0.497090;0.601613;,
  -0.000004;-0.505905;0.862589;,
  -0.625271;-0.497096;0.601608;,
  -0.872494;-0.488624;0.000004;,
  -0.625270;-0.497087;-0.601616;,
  -0.000002;-0.818156;-0.574996;,
  0.422049;-0.811553;-0.404049;,
  0.593199;-0.805056;0.000006;,
  0.422057;-0.811550;0.404048;,
  -0.000002;-0.818163;0.574987;,
  -0.422069;-0.811541;0.404054;,
  -0.593219;-0.805041;0.000006;,
  -0.422061;-0.811544;-0.404055;,
  0.000000;-0.960401;-0.278622;,
  0.206267;-0.958524;-0.196687;,
  0.291217;-0.956657;0.000005;,
  0.206271;-0.958522;0.196691;,
  0.000000;-0.960402;0.278619;,
  -0.206270;-0.958522;0.196690;,
  -0.291217;-0.956657;0.000005;,
  -0.206266;-0.958524;-0.196687;,
  -0.000000;-1.000000;0.000003;;
  209;
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;2,2,2,22;,
  4;23,14,10,10;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;6,0,0,7;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;8,8,8,8;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;11,24,24,23;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;13,13,11,23;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;25,26,26,27;,
  4;8,8,8,8;,
  4;3,3,3,3;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;16,16,5,5;,
  4;15,15,9,9;,
  4;17,17,17,17;,
  4;19,19,16,16;,
  4;18,18,15,15;,
  4;28,28,28,28;,
  4;20,21,29,30;,
  4;2,2,21,20;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;31,6,12,32;,
  4;4,4,4,4;,
  4;32,12,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  3;23,2,14;,
  3;1,1,1;,
  3;33,33,33;,
  3;22,2,20;,
  3;22,20,30;,
  3;22,30,34;,
  3;2,2,22;,
  3;3,3,35;,
  3;4,4,4;,
  3;1,1,1;,
  3;4,4,4;,
  3;1,1,1;,
  3;1,1,1;,
  3;3,3,3;,
  3;4,4,4;,
  3;4,4,4;,
  3;3,3,3;,
  3;36,36,36;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;0,0,0;,
  3;8,8,8;,
  3;31,32,37;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,25;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,8;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;7,12,6;,
  3;7,38,12;,
  3;0,0,0;,
  3;0,12,38;,
  3;1,1,1;,
  3;4,4,4;,
  3;1,39,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;4,4,4,4;,
  3;27,8,25;,
  3;1,1,1;,
  3;40,42,41;,
  3;40,43,42;,
  3;40,44,43;,
  3;40,45,44;,
  3;40,46,45;,
  3;40,47,46;,
  3;40,48,47;,
  3;40,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,49,57,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,57,65,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,65,73,80;,
  4;73,74,82,81;,
  4;74,75,83,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,73,81,88;,
  4;81,82,90,89;,
  4;82,83,91,90;,
  4;83,84,92,91;,
  4;84,85,93,92;,
  4;85,86,94,93;,
  4;86,87,95,94;,
  4;87,88,96,95;,
  4;88,81,89,96;,
  3;89,90,97;,
  3;90,91,97;,
  3;91,92,97;,
  3;92,93,97;,
  3;93,94,97;,
  3;94,95,97;,
  3;95,96,97;,
  3;96,89,97;;
 }
 MeshTextureCoords {
  389;
  0.948930;0.743060;,
  1.000000;0.743060;,
  1.000000;0.743060;,
  0.948930;0.743060;,
  0.948930;1.000000;,
  0.948930;0.806740;,
  0.948930;0.806740;,
  0.948930;1.000000;,
  0.437370;0.806740;,
  0.437370;1.000000;,
  0.437370;1.000000;,
  0.437370;0.806740;,
  0.389780;0.085060;,
  0.389780;0.000000;,
  0.437370;0.000000;,
  0.437370;0.085060;,
  0.437370;0.000000;,
  0.437370;0.085060;,
  0.948930;0.085060;,
  0.948930;0.000000;,
  1.000000;0.000000;,
  1.000000;0.085060;,
  1.000000;0.000000;,
  1.000000;0.085060;,
  0.389780;0.743060;,
  0.389780;0.806740;,
  0.389780;0.806740;,
  0.389780;0.743060;,
  1.000000;0.816650;,
  1.000000;1.000000;,
  0.914940;1.000000;,
  0.914940;0.816650;,
  0.948930;0.816650;,
  0.948930;0.305750;,
  0.948930;0.193260;,
  0.948930;1.000000;,
  0.948930;0.346070;,
  0.948930;0.256940;,
  0.000000;0.481710;,
  0.195540;0.481710;,
  0.195540;0.305750;,
  0.000000;0.305750;,
  0.195540;0.529820;,
  0.195540;0.816650;,
  0.237390;0.816650;,
  0.237390;1.000000;,
  0.085060;1.000000;,
  0.085060;0.816650;,
  0.000000;0.085060;,
  0.000000;0.085060;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.195540;0.000000;,
  0.195540;0.000000;,
  0.000000;0.673710;,
  0.000000;0.673710;,
  0.000000;0.743060;,
  0.000000;0.743060;,
  0.743060;0.154400;,
  0.673710;0.154400;,
  0.673710;0.075820;,
  0.743060;0.075820;,
  0.085060;0.529820;,
  0.000000;0.529820;,
  0.000000;0.529820;,
  0.085060;0.529820;,
  0.085060;0.816650;,
  0.000000;0.816650;,
  0.000000;0.806740;,
  0.000000;0.806740;,
  0.000000;0.806740;,
  0.000000;0.806740;,
  0.000000;0.085060;,
  0.000000;0.085060;,
  0.000000;0.604360;,
  0.000000;0.604360;,
  0.000000;0.604360;,
  0.000000;0.604360;,
  0.237390;0.390520;,
  0.237390;0.529820;,
  0.237390;0.529820;,
  0.237390;0.390520;,
  0.806740;0.075820;,
  0.806740;0.000000;,
  0.806740;0.000000;,
  0.806740;0.075820;,
  0.743060;0.000000;,
  0.673710;0.000000;,
  0.673710;0.000000;,
  0.743060;0.000000;,
  0.673710;0.075820;,
  0.743060;0.075820;,
  0.673710;0.154400;,
  0.085060;0.390520;,
  0.085060;0.390520;,
  0.806740;0.154400;,
  0.806740;0.154400;,
  0.806740;0.154400;,
  0.806740;0.154400;,
  0.673710;0.154400;,
  0.743060;0.154400;,
  0.743060;0.154400;,
  0.673710;0.154400;,
  0.743060;0.154400;,
  0.000000;0.549500;,
  0.000000;0.549500;,
  0.673710;0.208760;,
  0.743060;0.208760;,
  0.806740;0.208760;,
  0.806740;0.208760;,
  0.604360;0.260750;,
  0.604360;0.390520;,
  0.549500;0.390520;,
  0.549500;0.260750;,
  0.549500;0.208760;,
  0.604360;0.208760;,
  0.806740;0.260750;,
  0.806740;0.260750;,
  0.389780;0.085060;,
  0.389780;0.000000;,
  0.000000;0.085060;,
  0.195540;0.085060;,
  0.195540;0.085060;,
  0.000000;0.085060;,
  0.389780;0.237390;,
  0.389780;0.085060;,
  0.389780;0.237390;,
  0.292660;0.743060;,
  0.389780;0.743060;,
  0.292660;0.743060;,
  0.195540;0.743060;,
  0.195540;0.743060;,
  0.292660;0.806740;,
  0.195540;0.806740;,
  0.292660;0.806740;,
  0.195540;0.806740;,
  0.292660;0.085060;,
  0.292660;0.085060;,
  0.292660;0.806740;,
  0.195540;0.806740;,
  0.292660;1.000000;,
  0.195540;1.000000;,
  0.292660;0.529820;,
  0.292660;0.816650;,
  0.707340;0.816650;,
  0.707340;1.000000;,
  0.610220;1.000000;,
  0.610220;0.816650;,
  0.195540;0.914940;,
  0.000000;0.914940;,
  0.000000;0.914940;,
  0.195540;0.914940;,
  0.195540;1.000000;,
  0.195540;1.000000;,
  0.000000;1.000000;,
  0.195540;0.305750;,
  0.000000;0.305750;,
  0.195540;0.193260;,
  0.000000;0.193260;,
  0.195540;0.816650;,
  0.195540;0.305750;,
  0.195540;1.000000;,
  0.195540;0.193260;,
  0.195540;0.305750;,
  0.195540;0.193260;,
  0.326290;0.816650;,
  0.256940;0.816650;,
  0.256940;0.816650;,
  0.326290;0.816650;,
  0.326290;1.000000;,
  0.326290;1.000000;,
  0.256940;0.816650;,
  0.256940;1.000000;,
  0.256940;1.000000;,
  0.326290;0.816650;,
  0.326290;1.000000;,
  0.597530;0.816650;,
  0.597530;1.000000;,
  0.450500;1.000000;,
  0.450500;0.816650;,
  0.670490;0.816650;,
  0.670490;1.000000;,
  0.329510;0.529820;,
  0.329510;0.390520;,
  0.237390;0.260750;,
  0.329510;0.260750;,
  0.000000;0.329510;,
  0.000000;0.237390;,
  0.000000;0.237390;,
  0.000000;0.329510;,
  0.000000;0.237390;,
  0.000000;0.329510;,
  0.000000;0.237390;,
  0.000000;0.329510;,
  0.000000;0.237390;,
  0.000000;0.329510;,
  0.195540;0.329510;,
  0.000000;0.329510;,
  0.000000;0.237390;,
  0.195540;0.237390;,
  0.209130;0.816650;,
  0.209130;1.000000;,
  0.051070;1.000000;,
  0.051070;0.816650;,
  0.281300;0.816650;,
  0.281300;1.000000;,
  0.414550;0.816650;,
  0.491390;0.816650;,
  0.491390;1.000000;,
  0.414550;1.000000;,
  0.585450;0.000000;,
  0.718700;0.000000;,
  0.718700;0.000000;,
  0.585450;0.000000;,
  0.718700;0.085060;,
  0.585450;0.085060;,
  0.585450;1.000000;,
  0.585450;0.816650;,
  0.718700;0.816650;,
  0.718700;1.000000;,
  0.585450;0.806740;,
  0.718700;0.806740;,
  0.718700;1.000000;,
  0.585450;1.000000;,
  0.718700;1.000000;,
  0.585450;1.000000;,
  0.389780;0.806740;,
  0.292660;0.000000;,
  0.292660;0.000000;,
  0.389780;0.708380;,
  0.389780;0.673710;,
  0.634010;0.529820;,
  0.524020;0.529820;,
  0.291620;0.529820;,
  1.000000;0.673230;,
  0.914940;0.529820;,
  0.948930;0.415790;,
  0.256940;0.529820;,
  0.437370;0.774900;,
  0.869900;0.000000;,
  0.790870;0.000000;,
  0.948930;0.000000;,
  0.790870;0.085060;,
  0.547030;0.000000;,
  0.508610;0.000000;,
  0.472990;0.000000;,
  0.718700;0.673230;,
  0.585450;0.529820;,
  0.413570;1.000000;,
  0.341220;0.806740;,
  0.097770;0.806740;,
  0.000000;0.806740;,
  0.097770;0.806740;,
  0.195540;0.662720;,
  0.000000;0.646770;,
  0.000000;0.678670;,
  0.195540;0.695340;,
  0.000000;0.712010;,
  0.195540;0.736110;,
  0.000000;0.760220;,
  0.195540;0.783480;,
  0.806740;0.325640;,
  0.806740;0.390520;,
  0.806740;0.673230;,
  0.806740;0.529820;,
  0.806740;0.816650;,
  0.604360;0.460170;,
  0.549500;0.529820;,
  0.774900;0.816650;,
  0.743060;0.816650;,
  0.000000;0.774900;,
  0.000000;0.743060;,
  0.000000;0.806740;,
  0.000000;0.576930;,
  0.000000;0.639030;,
  0.000000;0.673710;,
  0.000000;0.708380;,
  0.283450;0.816650;,
  0.708380;0.260750;,
  0.161220;0.260750;,
  0.161220;0.816650;,
  0.161220;0.529820;,
  0.549500;0.181580;,
  0.604360;0.154400;,
  0.549500;0.037910;,
  0.604360;0.000000;,
  0.604360;0.075820;,
  0.549500;0.115110;,
  0.000000;0.639030;,
  0.237390;0.234750;,
  0.329510;0.208760;,
  0.085060;0.325640;,
  0.085060;0.260750;,
  0.000000;0.085060;,
  0.000000;0.085060;,
  0.000000;0.460170;,
  0.000000;0.161220;,
  0.707340;0.037910;,
  0.610220;0.075820;,
  0.610220;0.000000;,
  0.341220;0.000000;,
  0.974470;0.000000;,
  0.754780;0.000000;,
  0.586430;0.075820;,
  0.562630;0.000000;,
  0.804460;0.673230;,
  1.000000;0.529820;,
  1.000000;0.816650;,
  0.292660;0.329510;,
  0.292660;0.237390;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
