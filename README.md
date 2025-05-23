# university-dpl-t-floor
Jogo desenvolvido com o SGDK (Sega Genesis Development Kit) para o console de 16 bits Sega Mega Drive/Genesis, como parte da disciplina de Desenvolvimento para Plataformas Legadas.

<div align="center">
  <img src="https://github.com/user-attachments/assets/74daf7bb-e7d2-46cc-be50-3a0605a96b13" alt="Descrição da imagem" width="600"/>
</div>

== Setting up SGDK with Visual Studio Code

This small project will help to setup a working environment (tested in Windows) for Sega Mega Drive developing with SGDK toolchain and Visual Studio Code as prefered editor. Visual Studio Code will be setup with c/cpp code extension and tasks to build and clean the workspace.


=== Prerequisites

. Download link:https://github.com/Stephane-D/SGDK[SGDK]
. Download link:https://code.visualstudio.com/[Visual Studio Code]


=== Setup

* Extract SGDK to a prefered directory.
* Create an environment variable `GDK` pointing to the SGDK directory (e.g. "C:/dev/sgdk").
* Install Visual Studio Code.
* Install the following extensions in vsCode: `C/C++ for Visual Studio Code`
* Clone this github repo: `git clone https://github.com/pleft/SEGA_VSCode_Template.git`.

=== Usage

* Open `VSCode` and `File->Open Folder...` and choose the folder of the checked out repository.
* Folder `.vscode` contains two files: `c_cpp_properties.json` and `tasks.json`
    * In `c_cpp_properties.json` it is added the include folder of the `SGDK`: `"includePath": ["${GDK}/inc"]`
    * In `tasks.json` there are 2 tasks to help build and clean the project, `make` and `clean`. 
        - To run `make` task press `Ctrl-Shift-B`.
        - To run `make` or `clean` press `Ctrl-P` then write `task make` or `task clean`.
