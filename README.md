# 簡単な3Dシューティングゲーム

![ゲーム画像](pics/tank_game.png)

## 操作方法

上の矢印キーを押してタンクが進む。下の矢印キーを押してタンクが後退する。左と右の矢印キーを押してタンクが方向を変える。"space"キーを押してタンクが弾を発射する。すべてのエネミーが撃破されたあと、このゲームがクリアされた。

## シャドウをつける方法

このプログラムはOpenGLライブラリを利用して作成される。シャドウをつける方法は[LearnOpenGL](https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows)のサイトを見てください。

## 開発環境
* OS: Linux(Ubuntu 20.04)
* プログラミング言語: C++
* IDE: Visual Studio Code
* 外部ライブラリ
	* [OpenCV v4.5.5](https://opencv.org/)
	* [GLFW(OpenGLライブラリのひとつ) v3.4](https://www.glfw.org/)
	* [Glad](https://glad.dav1d.de/)
    * [stb_image](https://github.com/nothings/stb)
    * [assimp](https://github.com/assimp/assimp)
    * [glm](https://github.com/g-truc/glm)

## コンパイルとリンク
このプロジェクトはLinux環境で開発されたが、Windows環境でのコンパイルとリンクも可能です。
### [cmake](https://cmake.org/)とmakeコマンドでプログラムを作成する(Linux利用者向け)
cd ${path of the project} <br />cmake .  <br />make  以上のコマンドを実行すると、Linuxの実行ファイルが作成される。

### Visual Studioでプログラムを作成する(Windows利用者向け)
外部ライブラリのパスを指定すると、Visual Studioで[Windows実行ファイル](https://drive.google.com/drive/folders/1w9NHCNUg75JjbS4b-T0GRK2zdW4LRqgg)を作成することも可能です。

## 担当部分
このプロジェクトフォルダのglad.cと3Dモデル以外のすべてのファイルの作成者は私です。

## 利用素材
[Futuristic Trike (Version 2)](https://free3d.com/3d-model/futuristic-trike-verion-2-331974.html)

## Requirements

* glfw(version 2.20 or latter)
* OpenCV(version 4.40 or latter)
* stb_image(version 1.1 or latter)
* assimp(version 1.1.0 or latter)
* glm(vesion 2.2 or latter)
* glad(version 1.2.0 or latter)
