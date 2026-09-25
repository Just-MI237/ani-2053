Exercice 1 - La carte avant la marche

J'ai lance la commande jenga info dans le dossier Nkentseu.
Voici la sortie brute complete, de la premiere a la derniere ligne.

```

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

[NKCode] ATTENTION : aucun wheel Jenga trouve (dist/*.whl) -> le paquet n'aura PAS de Jenga embarque, et les boutons Construire/Executer seront inoperants. Produisez-le avec ./cri.sh dans le depot Jenga.
========================== Jenga Workspace: Nkentseu ===========================

Location: /home/justmaxit/Projets/Nkentseu
Entry file: /home/justmaxit/Projets/Nkentseu/Nkentseu.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows, Linux, macOS, Android, iOS, Web, HarmonyOS, XboxSeries, XboxOne
Target Architectures: x86_64, arm64, wasm32
Start project: Sandbox


Projects
------------------------------------------------------------
Name                                        Kind          Language   Test   External
====================================================================================
__Unitest__                                 StaticLib     C++        No     No
NKPlatform                                  StaticLib     C++        No     Yes
NKPlatform_Tests                            TestSuite     C++        Yes    Yes
NKCore                                      StaticLib     C++        No     Yes
NKCore_Tests                                TestSuite     C++        Yes    Yes
NKLogger                                    StaticLib     C++        No     Yes
NKLogger_Tests                              TestSuite     C++        Yes    Yes
NKMath                                      StaticLib     C++        No     Yes
NKMath_Tests                                TestSuite     C++        Yes    Yes
NKMemory                                    StaticLib     C++        No     Yes
NKMemory_Tests                              TestSuite     C++        Yes    Yes
NKContainers                                StaticLib     C++        No     Yes
NKContainers_Tests                          TestSuite     C++        Yes    Yes
NKImage                                     StaticLib     C++        No     Yes
NKImage_Tests                               TestSuite     C++        Yes    Yes
NKFont_Tests                                TestSuite     C++        Yes    Yes
NKFont                                      StaticLib     C++        No     Yes
NKAudio                                     StaticLib     C++        No     Yes
NKAudio_Audio_Tests                         TestSuite     C++        Yes    Yes
NKMedia                                     StaticLib     C++        No     Yes
NKTime                                      StaticLib     C++        No     Yes
NKTime_Tests                                TestSuite     C++        Yes    Yes
NKStream_Tests                              TestSuite     C++        Yes    Yes
NKStream                                    StaticLib     C++        No     Yes
NKThreading_Tests                           TestSuite     C++        Yes    Yes
NKThreading                                 StaticLib     C++        No     Yes
NKFileSystem_Tests                          TestSuite     C++        Yes    Yes
NKFileSystem                                StaticLib     C++        No     Yes
NKReflection                                StaticLib     C++        No     Yes
NKReflection_Tests                          TestSuite     C++        Yes    Yes
NKNetwork                                   StaticLib     C++        No     Yes
NKNetwork_Tests                             TestSuite     C++        Yes    Yes
NKSerialization_Bench_Tests                 TestSuite     C++        Yes    Yes
NKSerialization_Smoke_Tests                 TestSuite     C++        Yes    Yes
NKSerialization_ReflectObjContainer_Tests   TestSuite     C++        Yes    Yes
NKSerialization_ReflectPhase5_Tests         TestSuite     C++        Yes    Yes
NKSerialization                             StaticLib     C++        No     Yes
NKSerialization_ReflectPhase3_Tests         TestSuite     C++        Yes    Yes
NKSerialization_ReflectSerializer_Tests     TestSuite     C++        Yes    Yes
NKGlad                                      StaticLib     C          No     Yes
NKGLSlang                                   StaticLib     C++        No     Yes
NKSPIRVCross                                StaticLib     C++        No     Yes
NKMbedTLS                                   StaticLib     C          No     Yes
pybind11                                    StaticLib     C++        No     Yes
NKEvent                                     StaticLib     C++        No     Yes
NKEvent_Tests                               TestSuite     C++        Yes    Yes
NKWindow_Tests                              TestSuite     C++        Yes    Yes
NKWindow                                    StaticLib     C++        No     Yes
NKSL                                        StaticLib     C++        No     Yes
NKCanvas_Tests                              TestSuite     C++        Yes    Yes
NKCanvas                                    StaticLib     C++        No     Yes
NKRHI_Tests                                 TestSuite     C++        Yes    Yes
NKRHI                                       StaticLib     C++        No     Yes
NKUI_Tests                                  TestSuite     C++        Yes    Yes
NKUI                                        StaticLib     C++        No     Yes
NKGui                                       StaticLib     C++        No     Yes
NKEditorKit                                 StaticLib     C++        No     Yes
Unkeny                                      StaticLib     C++        No     Yes
NKCode                                      WindowedApp   C++        No     Yes
NkFileWorkerTest                            ConsoleApp    C++        No     Yes
NkPdfRasterTest                             ConsoleApp    C++        No     Yes
NkPdfRenderProbe                            ConsoleApp    C++        No     Yes
NkPdfProbe                                  ConsoleApp    C++        No     Yes
NK3DModeler                                 WindowedApp   C++        No     Yes
NKImGuiIntegration                          StaticLib     C++        No     Yes
NKUIIntegration                             StaticLib     C++        No     Yes
NKGuiIntegration                            StaticLib     C++        No     Yes
NKCamera                                    StaticLib     C++        No     Yes
NKCamera_Tests                              TestSuite     C++        Yes    Yes
NKCollision                                 StaticLib     C++        No     Yes
NKCollision_Tests                           TestSuite     C++        Yes    Yes
NKPhysics_Tests                             TestSuite     C++        Yes    Yes
NKPhysics                                   StaticLib     C++        No     Yes
NKAnima                                     StaticLib     C++        No     Yes
NKAnima_Tests                               TestSuite     C++        Yes    Yes
NKNavigation_Tests                          TestSuite     C++        Yes    Yes
NKNavigation                                StaticLib     C++        No     Yes
NKXR                                        StaticLib     C++        No     Yes
NKXR_Xr_Tests                               TestSuite     C++        Yes    Yes
NKXR_Ar_Tests                               TestSuite     C++        Yes    Yes
NkRHIInterpTest                             WindowedApp   C++        No     Yes
cr2d_Tests                                  TestSuite     C++        Yes    Yes
firsttriangle_Tests                         TestSuite     C++        Yes    Yes
NewGeneration_Tests                         TestSuite     C++        Yes    Yes
cr2d                                        WindowedApp   C++        No     Yes
FontLoad                                    WindowedApp   C++        No     Yes
NkSpriteDemo                                WindowedApp   C++        No     Yes
r2d01_Tests                                 TestSuite     C++        Yes    Yes
RendererRHI_Tests                           TestSuite     C++        Yes    Yes
renderdemo                                  WindowedApp   C++        No     Yes
NKRHIDemo_Tests                             TestSuite     C++        Yes    Yes
NkSpriteDemo_Tests                          TestSuite     C++        Yes    Yes
NkRHIDemoFullImage_Tests                    TestSuite     C++        Yes    Yes
NkSWRasterPrimitives                        WindowedApp   C++        No     Yes
firsttriangle                               WindowedApp   C++        No     Yes
Gamepad_Tests                               TestSuite     C++        Yes    Yes
NkFDV2                                      WindowedApp   C++        No     Yes
NkFDV2_Tests                                TestSuite     C++        Yes    Yes
gltftest_Tests                              TestSuite     C++        Yes    Yes
NkFontDemo                                  WindowedApp   C++        No     Yes
NkCanvasDemo                                WindowedApp   C++        No     Yes
RendererRHI                                 WindowedApp   C++        No     Yes
NkRHIInterpTest_Tests                       TestSuite     C++        Yes    Yes
NkRHIDemoFull                               WindowedApp   C++        No     Yes
r2d01                                       WindowedApp   C++        No     Yes
gltftest                                    WindowedApp   C++        No     Yes
NkCanvasDemo_Tests                          TestSuite     C++        Yes    Yes
renderdemo_Tests                            TestSuite     C++        Yes    Yes
NKRHIDemo                                   WindowedApp   C++        No     Yes
Gamepad                                     WindowedApp   C++        No     Yes
NkDrawableDemo_Tests                        TestSuite     C++        Yes    Yes
NkDrawableDemo                              WindowedApp   C++        No     Yes
NkFontDemo_Tests                            TestSuite     C++        Yes    Yes
NkSWRasterPrimitives_Tests                  TestSuite     C++        Yes    Yes
NkRHIDemoFullImage                          WindowedApp   C++        No     Yes
NkRHIDemoFull_Tests                         TestSuite     C++        Yes    Yes
NewGeneration                               WindowedApp   C++        No     Yes
ConquerorProto                              WindowedApp   C++        No     Yes
NkAudioDemo                                 ConsoleApp    C++        No     Yes
NkCameraDemos                               WindowedApp   C++        No     Yes
NkEauEcsProbe                               ConsoleApp    C++        No     Yes
SandboxNKFileSystem                         ConsoleApp    C++        No     Yes
SandboxNKLogger                             ConsoleApp    C++        No     Yes
SandboxNKReflection                         ConsoleApp    C++        No     Yes
SandboxNKNetwork                            ConsoleApp    C++        No     Yes
SandboxNKSerialization                      ConsoleApp    C++        No     Yes
SandboxNKArchive                            ConsoleApp    C++        No     Yes
NKRenderer                                  StaticLib     C++        No     Yes
NKRenderer_Tests                            TestSuite     C++        Yes    Yes
NKVFX                                       StaticLib     C++        No     Yes
NKTensor_Tests                              TestSuite     C++        Yes    Yes
NKTensor                                    StaticLib     C++        No     Yes
NKAutograd                                  StaticLib     C++        No     Yes
NKAutograd_Tests                            TestSuite     C++        Yes    Yes
NKNN                                        StaticLib     C++        No     Yes
NKNN_Tests                                  TestSuite     C++        Yes    Yes
NKOptim_Tests                               TestSuite     C++        Yes    Yes
NKOptim                                     StaticLib     C++        No     Yes
NKData                                      StaticLib     C++        No     Yes
NKData_Tests                                TestSuite     C++        Yes    Yes
NKTrain                                     StaticLib     C++        No     Yes
NKTrain_Tests                               TestSuite     C++        Yes    Yes
NKInfer                                     StaticLib     C++        No     Yes
NKInfer_Tests                               TestSuite     C++        Yes    Yes
NKRL_Tests                                  TestSuite     C++        Yes    Yes
NKRL                                        StaticLib     C++        No     Yes
NKAgent_Tests                               TestSuite     C++        Yes    Yes
NKAgent                                     StaticLib     C++        No     Yes
NKEvolve                                    StaticLib     C++        No     Yes
NKEvolve_Tests                              TestSuite     C++        Yes    Yes
NKCivilization_Tests                        TestSuite     C++        Yes    Yes
NKCivilization                              StaticLib     C++        No     Yes
NKEmbodied                                  StaticLib     C++        No     Yes
NKEmbodied_Tests                            TestSuite     C++        Yes    Yes
NKGen_Tests                                 TestSuite     C++        Yes    Yes
NKGen                                       StaticLib     C++        No     Yes
NKGpt                                       StaticLib     C++        No     Yes
NKSpeech                                    StaticLib     C++        No     Yes
NKTensorDemo                                ConsoleApp    C++        No     Yes
NkSLComputeCheck                            ConsoleApp    C++        No     Yes
NkGpuProbe                                  ConsoleApp    C++        No     Yes
NkFluidGridProbe                            ConsoleApp    C++        No     Yes
NkFluidEcsProbe                             ConsoleApp    C++        No     Yes
NkComputeNkSL                               ConsoleApp    C++        No     Yes
NkTensorGpuTest                             ConsoleApp    C++        No     Yes
NKGpuBenchTest                              ConsoleApp    C++        No     Yes
NKConvBenchTest                             ConsoleApp    C++        No     Yes
NKConvResidentBench                         ConsoleApp    C++        No     Yes
NKMlpResidentBench                          ConsoleApp    C++        No     Yes
NKMnistGpuTrain                             ConsoleApp    C++        No     Yes
NKRebasinTest                               ConsoleApp    C++        No     Yes
NKMnistCnnGpuTrain                          ConsoleApp    C++        No     Yes
NKTransformerTest                           ConsoleApp    C++        No     Yes
NKGptTrain                                  ConsoleApp    C++        No     Yes
NKIlyana                                    ConsoleApp    C++        No     Yes
NKAutogradTest                              ConsoleApp    C++        No     Yes
NKNNTest                                    ConsoleApp    C++        No     Yes
NKConvTest                                  ConsoleApp    C++        No     Yes
NKDataTest                                  ConsoleApp    C++        No     Yes
NKBpeTest                                   ConsoleApp    C++        No     Yes
NKLlamaBlockTest                            ConsoleApp    C++        No     Yes
NKRebasinTransformer                        ConsoleApp    C++        No     Yes
NKTrainTest                                 ConsoleApp    C++        No     Yes
NKFp16Test                                  ConsoleApp    C++        No     Yes
NKRnnCtcTest                                ConsoleApp    C++        No     Yes
NKASRTest                                   ConsoleApp    C++        No     Yes
NKImageCodecTest                            ConsoleApp    C++        No     Yes
NkEditableMeshDemo                          ConsoleApp    C++        No     Yes
NkLocomotionDemo                            ConsoleApp    C++        No     Yes
NkAssetIODemo                               ConsoleApp    C++        No     Yes
NkQualityProfileTest                        ConsoleApp    C++        No     Yes
NkSystemsRevivalTest                        ConsoleApp    C++        No     Yes
NkFBXParityDemo                             ConsoleApp    C++        No     Yes
NkSVGImportDemo                             ConsoleApp    C++        No     Yes
NKMeshAITest                                ConsoleApp    C++        No     Yes
NKEditMeshHarness                           ConsoleApp    C++        No     Yes
NkAnimaTest                                 ConsoleApp    C++        No     Yes
NkMicRecord                                 ConsoleApp    C++        No     Yes
NKSpeechTest                                ConsoleApp    C++        No     Yes
NKTTSTrain                                  ConsoleApp    C++        No     Yes
NkVoiceLoopDemo                             ConsoleApp    C++        No     Yes
NKSpeechFeatureDemo                         ConsoleApp    C++        No     Yes
NKMediaTest                                 ConsoleApp    C++        No     Yes
NkVideoReadTest                             ConsoleApp    C++        No     Yes
NkAudioPlayer                               WindowedApp   C++        No     Yes
NkVideoPlayer                               WindowedApp   C++        No     Yes
NKOpusRef                                   ConsoleApp    C++        No     Yes
NKVideoTest                                 ConsoleApp    C++        No     Yes
NKInferTest                                 ConsoleApp    C++        No     Yes
NKGGUFInspectTest                           ConsoleApp    C++        No     Yes
NKLLMInferTest                              ConsoleApp    C++        No     Yes
NKQwenTokenizerTest                         ConsoleApp    C++        No     Yes
NKQwen2BackwardTest                         ConsoleApp    C++        No     Yes
NKQwen2SftTest                              ConsoleApp    C++        No     Yes
NKQ4MatmulTest                              ConsoleApp    C++        No     Yes
NKQwen2GpuTest                              ConsoleApp    C++        No     Yes
NKQwen2Chat                                 ConsoleApp    C++        No     Yes
NKQwen2SftGpuTest                           ConsoleApp    C++        No     Yes
NKQwen2Train                                ConsoleApp    C++        No     Yes
NKQwen2Ask                                  ConsoleApp    C++        No     Yes
NKRLTest                                    ConsoleApp    C++        No     Yes
NKAgentTest                                 ConsoleApp    C++        No     Yes
NKAgentLLMTest                              ConsoleApp    C++        No     Yes
NkAgentEcsDemo                              ConsoleApp    C++        No     Yes
NKEmbodiedTest                              ConsoleApp    C++        No     Yes
NKEvolveTest                                ConsoleApp    C++        No     Yes
NKEvolveNNTest                              ConsoleApp    C++        No     Yes
NKCivilizationTest                          ConsoleApp    C++        No     Yes
NKCivilizationSocialTest                    ConsoleApp    C++        No     Yes
NKCivilizationScaleTest                     ConsoleApp    C++        No     Yes
NKGenTest                                   ConsoleApp    C++        No     Yes
NKVAETest                                   ConsoleApp    C++        No     Yes
NKMnistVAETest                              ConsoleApp    C++        No     Yes
NKMnistConvVAETest                          ConsoleApp    C++        No     Yes
NKConvVAETest                               ConsoleApp    C++        No     Yes
NKDiffusionTest                             ConsoleApp    C++        No     Yes
NKVoxelGenTest                              ConsoleApp    C++        No     Yes
NKObjectGenTest                             ConsoleApp    C++        No     Yes
NKGen3DTest                                 ConsoleApp    C++        No     Yes
NKGenMeshTest                               ConsoleApp    C++        No     Yes
NKSmoothMeshTest                            ConsoleApp    C++        No     Yes
NKMatTypeResetTest                          ConsoleApp    C++        No     Yes
NkMsaaContractCheck                         ConsoleApp    C++        No     Yes
NkMsaaDeviceCheck                           ConsoleApp    C++        No     Yes
NkMsaaVulkanCheck                           ConsoleApp    C++        No     Yes
NkIblEnabledCheck                           ConsoleApp    C++        No     Yes
NkDebugOverlayCheck                         ConsoleApp    C++        No     Yes
NKEditTargetTest                            ConsoleApp    C++        No     Yes
NKMeshRenderTest                            ConsoleApp    C++        No     Yes
Model                                       WindowedApp   C++        No     Yes
NkSLCheck                                   ConsoleApp    C++        No     Yes
NkMatGraphCheck                             ConsoleApp    C++        No     Yes
NkMatGraphDemo                              ConsoleApp    C++        No     Yes
NKPA                                        WindowedApp   C++        No     Yes
NKECS_ReflectBridge_Tests                   TestSuite     C++        Yes    Yes
NKECS                                       StaticLib     C++        No     Yes
NKECS_EntitySerialization_Tests             TestSuite     C++        Yes    Yes
Noge_Tests                                  TestSuite     C++        Yes    Yes
Noge                                        StaticLib     C++        No     Yes
Nogee                                       WindowedApp   C++        No     Yes
NKEditorKitDemo                             WindowedApp   C++        No     Yes
NKEditorKitTest                             ConsoleApp    C++        No     Yes
NKUIDesign                                  WindowedApp   C++        No     Yes
ConquerorLab                                WindowedApp   C++        No     Yes
NkRef                                       WindowedApp   C++        No     Yes
NkAnimaEditor                               WindowedApp   C++        No     Yes
ImGuiRef                                    WindowedApp   C++        No     Yes
NKGuiDrawTest                               ConsoleApp    C++        No     Yes
NKGuiDemo                                   WindowedApp   C++        No     Yes
NKViewportDemo                              WindowedApp   C++        No     Yes
PV3DE                                       WindowedApp   C++        No     Yes
Pong                                        WindowedApp   C++        No     Yes
Nkoung                                      WindowedApp   C++        No     Yes
Mou                                         WindowedApp   C++        No     Yes
NkImeTest                                   WindowedApp   C++        No     Yes
NkImageDemo                                 WindowedApp   C++        No     Yes
RihenDefi                                   WindowedApp   C++        No     Yes
NkAudioECSDemo                              ConsoleApp    C++        No     Yes
NkNetWorldDemo                              ConsoleApp    C++        No     Yes
NkNavDemo                                   ConsoleApp    C++        No     Yes
NkNavCoreDemo                               ConsoleApp    C++        No     Yes
NkHotReloadDemo                             ConsoleApp    C++        No     Yes
NkUIHudDemo                                 ConsoleApp    C++        No     Yes
Tuto04Camera                                WindowedApp   C++        No     Yes
Tuto05Meshes                                WindowedApp   C++        No     Yes
Tuto03Scene                                 WindowedApp   C++        No     Yes
Tuto01Fenetre                               WindowedApp   C++        No     Yes
Tuto02Renderer                              WindowedApp   C++        No     Yes
NKXRDemo                                    WindowedApp   C++        No     Yes
NKARDemo                                    WindowedApp   C++        No     Yes
GemCrush                                    WindowedApp   C++        No     Yes
NkDames                                     WindowedApp   C++        No     Yes
NkEchecs                                    WindowedApp   C++        No     Yes
NkLudo                                      WindowedApp   C++        No     Yes
UnkenyEditor                                WindowedApp   C++        No     Yes
MonEssai                                    ConsoleApp    C++        No     Yes


Available Toolchains
------------------------------------------------------------
Name          Family   Target OS   Arch     Env
=================================================
host-clang    clang    Linux       x86_64   gnu
host-gcc      gcc      Linux       x86_64   gnu
clang-mingw   clang    Windows     x86_64   mingw


Daemon
------------------------------------------------------------
Status: Not running

```

Analyse

Nombre de projets par type :
- StaticLib : 91
- TestSuite : 71
- ConsoleApp : 85
- WindowedApp : 65
Total : 312 projets, ce qui correspond a la somme des quatre nombres.

Nombre de chaines de compilation : 3, nommees host-clang, host-gcc
et clang-mingw, visibles dans la section Available Toolchains.

Projet de demarrage : Sandbox, indique par la ligne Start project.

Fichier racine du workspace : Nkentseu.jenga, indique par la ligne
Entry file.

Reponse a la question sur le projet de demarrage

Le projet de demarrage est celui qui est lance quand on tape jenga
run sans preciser --project. Si on ne le precise pas et qu'on ne
donne pas non plus --project sur la ligne de commande, Jenga ne
sait pas quel binaire executer et refuse de continuer. C'est ce
que montre la sortie : Start project: Sandbox est la valeur par
defaut choisie par le workspace.

Mesure faite le 25/09/2026.
Version de Jenga : 2.8.0.
Dernier commit du depot Nkentseu : 9c3fad3, date du 2026-09-13.
