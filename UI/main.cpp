// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#define _CRT_SECURE_NO_WARNINGS

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imGui/implot.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <iostream>
#include <Windows.h>				//TwinCAT ADS 통신에 사용되는 여러 datatype에 대한 정의가 여기에 되어있으므로 넣어주어야 합니다.
#include "ADSDataStructure.h"		//TwinCAT ADS 통신에 사용되는 구조체에 대한 정의가 되어 있는 헤더 파일입니다.
#include "updateADS.h"				//ADS 통신이 정의되어있는 파일입니다.
#include <stdlib.h>
#include <time.h>
#include "Utils.h"
#include <cstdlib>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    srand(time(NULL));
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1440, 1080, "Exoskeleton Equipment & Rehabilitation Robot.", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsClassic();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    AdsRead ReadData;
    AdsWrite WriteData;

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Global state
    bool show_demo_window = true;
    bool show_experimenter_window = false;
    bool show_imu_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float Left = 0, Right = 0, pos_avg = 0;
    bool PosRst = false;
    bool btn_rst = false;
    bool btn_minmax_set = false;
    bool isMinSet = false, isMaxSet = false;
    float minPos, maxPos;
    float range = PI * 0.5;
    float desired = 0;
    int acquired_cnt = 0;
    float diff;
    int torque = 0, speed = 0;
    float pos_prev_L, pos_prev_R, pos_prev_avg;
    float PosOff_L = 0, PosOff_R = 0;
    float PosOff_Sum_L = 0, PosOff_Sum_R = 0;
    float Imu_PosOff_Sum = 0, Imu_PosOff = 0;
    float vel_cal_L = 0, vel_cal_R = 0;
    float vel_prev_L = 0, vel_prev_R = 0;
    float pos_abs_diff = 400 * INC_TO_RAD;

    float Acc_cal_L = 0, Acc_cal_R = 0;
    float Acc_prev_L = 0, Acc_prev_R = 0;

    float torque_prev = 0, torque_curr = 0, torque_filtered = 0, torque_raw_prev = 0, torque_offset = 0, torque_Off_Sum = 0;

    float sEMG_raw_ch1 = 0, sEMG_raw_ch2 = 0;
    float sEMG_Off_ch1 = 0, sEMG_Off_ch2 = 0;
    float sEMG_Off_Sum_ch1 = 0, sEMG_Off_Sum_ch2 = 0;
    float sEMG_abs_Sum_ch1 = 0, sEMG_abs_Sum_ch2 = 0;
    float sEMG_MAV_Sum_ch1 = 0, sEMG_MAV_Sum_ch2 = 0;
    float sEMG_MAV_Mean_ch1 = 0, sEMG_MAV_Mean_ch2 = 0;
    float sEMG_MAV_Fin_ch1 = 0, sEMG_MAV_Fin_ch2 = 0;
    float sEMG_abs_ch1 = 0, sEMG_abs_ch2 = 0;
    float sEMG_MAV_ch1 = 0, sEMG_MAV_ch2 = 0;
    float sEMG_abs_arr_ch1[MAV_WINSIZE] = { 0 };
    float sEMG_abs_arr_ch2[MAV_WINSIZE] = { 0 };
    float sEMG_MAV_arr_ch1[MAV_WINSIZE] = { 0 };
    float sEMG_MAV_arr_ch2[MAV_WINSIZE] = { 0 };

    int sEMG_cnt = 0;
    enum sEMG_Mode {
        RAW,
        ABS,
        MAV
    };
    static int semg_mode = 0;
    enum Rehab_Mode {
        passive,
        active
    };
    static int rehab_mode = 0;
    int rst_cnt = 0;

    bool start_flag = true;


    //Experimenter State
    bool pos_online = false;
    bool exp_rst = false;
    static ScrollingBuffer Pos_buf[2];
    static ScrollingBuffer Vel_buf[2];
    static ScrollingBuffer Acc_buf[2];
    static ScrollingBuffer sEMG_buf[2];
    static ScrollingBuffer Tor_buf;
    static ScrollingBuffer IMU_Pos_buf[3];
    static ScrollingBuffer IMU_Vel_buf[3];
    static ScrollingBuffer IMU_Acc_buf[3];
    static ScrollingBuffer IMU_sEMG_buf[3];
    char label[32];
    static float t_p;


    //initialize IMU
    InitMatch();
    OpenMatch();

    int base_imu = 1;
    int measure_imu = 2;

    float base_pos[3] = { 0 }, measure_pos[3] = { 0 };
    float base_pos_prev[3] = { 0 }, measure_pos_prev[3] = { 0 };
    float base_vel[3] = { 0 }, measure_vel[3] = { 0 };
    float base_vel_prev[3] = { 0 }, measure_vel_prev[3] = { 0 };
    float base_acc[3] = { 0 }, measure_acc[3] = { 0 };
    float base_acc_prev[3] = { 0 }, measure_acc_prev[3] = { 0 };
    float base_sEmg[3] = { 0 }, measure_sEmg[3] = { 0 };

    float EulerInput[SEN_NUM * 3] = { 0 };              //# Senser * 3 (X, Y, Z - Axis)
    float GyroInput[SEN_NUM * 3] = { 0 };               //# Senser * 3 (X, Y, Z - Axis)
    float AccInput[SEN_NUM * 3] = { 0 };               //# Senser * 3 (X, Y, Z - Axis)
    float EMGInput[SEN_NUM * 3] = { 0 };               //# Senser * 3 (Raw, Offset, Filtered value)


    //Data Save
    std::string SaveFolderPath;
    std::string SaveFolderName;
    time_t now = time(NULL);
    tm* ptm;

    char timebuf[64] = "";
    
    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        /*----------Cyclic Updates----------*/

        pos_prev_L = Left;
        pos_prev_R = Right;
        pos_prev_avg = (pos_prev_L + pos_prev_R) / 2;
        vel_prev_L = ReadData.VelAct_L;
        vel_prev_R = ReadData.VelAct_R;
        //vel_prev_L = vel_cal_L;
        //vel_prev_R = vel_cal_R;
        Acc_prev_L = Acc_cal_L;
        Acc_prev_R = Acc_cal_R;
        torque_prev = torque_filtered;
        torque_raw_prev = torque_curr;

        // Read/Write data from twinCAT
        ReadADS(&ReadData);
        WriteADS(&WriteData);
        
        //std::cout << ReadData.Torque_raw << std::endl;
        //std::cout << torque_filtered << std::endl;

        //------------OFFSET------------//

        Left = ReadData.PosInc_L - PosOff_L;
        Right = ReadData.PosInc_R - PosOff_R;

        torque_curr = ReadData.Torque_raw - torque_offset;

        //------------DERIVATIVE & FILTERING------------//

        vel_cal_L = FilteredDerivative(pos_prev_L, Left, vel_prev_L, 2 * PI * 2, (float)T_S);
        vel_cal_R = FilteredDerivative(pos_prev_R, Right, vel_prev_R, 2 * PI * 2, (float)T_S);

        //Acc_cal_L = FilteredDerivative(vel_prev_L, vel_cal_L, Acc_prev_L, 2 * PI * 80, (float)T_S);
        //Acc_cal_R = FilteredDerivative(vel_prev_R, vel_cal_R, Acc_prev_R, 2 * PI * 80, (float)T_S);

        Acc_cal_L = FilteredDerivative(vel_prev_L, vel_cal_L, Acc_prev_L, 2 * PI * 100, (float)T_S);
        Acc_cal_R = FilteredDerivative(vel_prev_R, vel_cal_R, Acc_prev_R, 2 * PI * 100, (float)T_S);

        pos_avg = (Left + Right) / 2;
        torque_filtered = LowPassFilter(torque_raw_prev, torque_curr, torque_prev, 100, (float)T_S);

        if (PosRst || exp_rst || start_flag) {
            if (rst_cnt < RST_CYCLE) {
                PosOff_Sum_L += ReadData.PosInc_L;
                PosOff_Sum_R += ReadData.PosInc_R;
                torque_Off_Sum += ReadData.Torque_raw;
                sEMG_Off_Sum_ch1 += ReadData.sEMG_Ch1;
                sEMG_Off_Sum_ch2 += ReadData.sEMG_Ch2;
                Imu_PosOff_Sum += EulerInput[3 * measure_imu];
                rst_cnt++;
            }
            else if (rst_cnt == RST_CYCLE) {
                PosOff_L = PosOff_Sum_L / RST_CYCLE;
                PosOff_R = PosOff_Sum_R / RST_CYCLE;
                torque_offset = torque_Off_Sum / RST_CYCLE;
                sEMG_Off_ch1 = sEMG_Off_Sum_ch1 / RST_CYCLE;
                sEMG_Off_ch2 = sEMG_Off_Sum_ch2 / RST_CYCLE;
                Imu_PosOff = Imu_PosOff_Sum / RST_CYCLE;
                rst_cnt++;
            }
            else {
                PosRst = false;
                exp_rst = false;
                start_flag = false;
                minPos = ReadData.PosInc_R;
                maxPos = ReadData.PosInc_R;
                rst_cnt = 0;
                PosOff_Sum_L = 0;
                PosOff_Sum_R = 0;
                torque_Off_Sum = 0;
                sEMG_Off_Sum_ch1 = 0;
                sEMG_Off_Sum_ch2 = 0;
                Imu_PosOff_Sum = 0;
            }
        }

        //------------ sEMG Filtering ------------//
        sEMG_raw_ch1 = ReadData.sEMG_Ch1 - sEMG_Off_ch1;
        sEMG_raw_ch2 = ReadData.sEMG_Ch2 - sEMG_Off_ch2;
        sEMG_abs_ch1 = sEMG_raw_ch1 >= 0 ? sEMG_raw_ch1 : -sEMG_raw_ch1;
        sEMG_abs_ch2 = sEMG_raw_ch2 >= 0 ? sEMG_raw_ch2 : -sEMG_raw_ch2;
        sEMG_abs_arr_ch1[sEMG_cnt] = sEMG_abs_ch1;
        sEMG_abs_arr_ch2[sEMG_cnt] = sEMG_abs_ch2;
        sEMG_abs_Sum_ch1 = 0;
        sEMG_abs_Sum_ch2 = 0;
        for (int i = 0; i < MAV_WINSIZE; i++) {
            sEMG_abs_Sum_ch1 += sEMG_abs_arr_ch1[i];
            sEMG_abs_Sum_ch2 += sEMG_abs_arr_ch2[i];
        }
        sEMG_MAV_ch1 = sEMG_abs_Sum_ch1 / MAV_WINSIZE;
        sEMG_MAV_ch2 = sEMG_abs_Sum_ch2 / MAV_WINSIZE;
        sEMG_MAV_arr_ch1[sEMG_cnt] = sEMG_MAV_ch1;
        sEMG_MAV_arr_ch2[sEMG_cnt] = sEMG_MAV_ch2;
        sEMG_MAV_Sum_ch1 = 0;
        sEMG_MAV_Sum_ch2 = 0;
        for (int i = 0; i < MAV_WINSIZE; i++) {
            sEMG_MAV_Sum_ch1 += sEMG_MAV_arr_ch1[i];
            sEMG_MAV_Sum_ch2 += sEMG_MAV_arr_ch2[i];
        }
        sEMG_MAV_Mean_ch1 = sEMG_MAV_Sum_ch1 / MAV_WINSIZE;
        sEMG_MAV_Mean_ch2 = sEMG_MAV_Sum_ch2 / MAV_WINSIZE;

        sEMG_MAV_Fin_ch1 = sEMG_MAV_ch1 - sEMG_MAV_Mean_ch1;
        sEMG_MAV_Fin_ch2 = sEMG_MAV_ch2 - sEMG_MAV_Mean_ch2;

        sEMG_cnt = (sEMG_cnt + 1) % MAV_WINSIZE;

        diff = desired - Right;


        //------------Control Part------------//

        if (pos_online && -0.1 <= diff && diff <= 0.1) {
            acquired_cnt++;
            if (acquired_cnt >= 100) {
                desired = range * ((float)(rand() % 100) / 100);
                //std::cout << desired << std::endl;
                acquired_cnt = 0;
            }
        }
        else {
            acquired_cnt = 0;
        }

        if (rehab_mode == active) {
            if (torque > 0) {
                WriteData.Controlword_L = 0x0f;
                WriteData.Controlword_R = 0x0f;
                if (ReadData.VelAct_R < -0.05) {
                    //WriteData.desired_torque_L = torque * 10;
                    WriteData.desired_torque_R = torque * -10;
                    //std::cout << "moving downward" << std::endl;
                }
                else if (ReadData.VelAct_R > 0.05) {
                    //WriteData.desired_torque_L = torque * -10;
                    WriteData.desired_torque_R = torque * 10;
                    //std::cout << "moving upward" << std::endl;
                }
                else {
                    WriteData.desired_torque_R = 0;
                    //std::cout << "stuck" << std::endl;
                }
            }
            else if (torque < 0) {
                WriteData.Controlword_L = 0x0f;
                WriteData.Controlword_R = 0x0f;
                //printf(" % d", time(NULL));
                if (diff > 0) {
                    WriteData.desired_torque_L = torque * 0;
                    WriteData.desired_torque_R = torque * sqrt(diff) * 20;
                    std::cout << WriteData.desired_torque_R << std::endl;
                }
                else if (diff < 0) {
                    WriteData.desired_torque_L = torque * 0;
                    WriteData.desired_torque_R = torque * sqrt(-1 * diff) * -20;
                    std::cout << WriteData.desired_torque_R << std::endl;
                }
                else {
                    std::cout << "stuck" << std::endl;
                }
            }
            else {
                WriteData.Controlword_L = 0x06;
                WriteData.Controlword_R = 0x06;
            }
        }
        else if(rehab_mode == passive && pos_online) {
            WriteData.Controlword_R = 0x0f;
            std::cout << diff << " " << desired << " " << Right << " " << vel_cal_R << " " << WriteData.desired_torque_R << std::endl;
            if (diff < 0) {
                if (vel_cal_R < -1) {               //너무 빠를 때.
                    WriteData.desired_torque_R = WriteData.desired_torque_R - 5;
                }
                else if (-1 <= vel_cal_R && vel_cal_R < -0.5) {
                    WriteData.desired_torque_R = WriteData.desired_torque_R - 1;
                }
                else if (-0.5 <= vel_cal_R && vel_cal_R < 0) {
                    WriteData.desired_torque_R = WriteData.desired_torque_R + 1;
                }
                else {
                    WriteData.desired_torque_R = WriteData.desired_torque_R + 5;
                }
            }
            else if (diff > 0) {
                if (vel_cal_R > 1) {               //너무 빠를 때.
                    WriteData.desired_torque_R = WriteData.desired_torque_R + 5;
                }
                else if (1 >= vel_cal_R && vel_cal_R > 0.5) {
                    WriteData.desired_torque_R = WriteData.desired_torque_R + 1;
                }
                else if (0.5 >= vel_cal_R && vel_cal_R > 0) {
                    WriteData.desired_torque_R = WriteData.desired_torque_R - 1;
                }
                else {
                    WriteData.desired_torque_R = WriteData.desired_torque_R - 5;
                }
            }
            else {

            }
        }
        else {
            WriteData.Controlword_R = 0x06;
        }

        
        
        //------------IMU------------//



        if (show_imu_window) {
            LufGetAllEuler_deg(DEVICE_ID, EulerInput, sizeof(EulerInput));
            LufGetAllGyro_dps(DEVICE_ID, GyroInput, sizeof(GyroInput));
            LufGetAllEmg_V(DEVICE_ID, EMGInput, sizeof(EMGInput));
            LufGetAllAccel_g(DEVICE_ID, AccInput, sizeof(AccInput));

            for (int i = 0; i < 3; i++) {
                base_pos_prev[i] = base_pos[i];
                measure_pos_prev[i] = measure_pos[i];
            }

            base_pos[0] = EulerInput[3 * base_imu] * DEG_TO_RAD;
            base_pos[1] = EulerInput[3 * base_imu + 1] * DEG_TO_RAD;
            base_pos[2] = EulerInput[3 * base_imu + 2] * DEG_TO_RAD;
            measure_pos[0] = (-1 * EulerInput[3 * measure_imu] + Imu_PosOff) * DEG_TO_RAD;
            measure_pos[1] = -1 * EulerInput[3 * measure_imu + 1] * DEG_TO_RAD;
            measure_pos[2] = -1 * EulerInput[3 * measure_imu + 2] * DEG_TO_RAD;

            for (int i = 0; i < 3; i++) {
                base_vel_prev[i] = base_vel[i];
                measure_vel_prev[i] = measure_vel_prev[i];
            }

            /*base_vel[0] = GyroInput[3 * base_imu] * DEG_TO_RAD;
            base_vel[1] = GyroInput[3 * base_imu + 1] * DEG_TO_RAD;
            base_vel[2] = GyroInput[3 * base_imu + 2] * DEG_TO_RAD;
            measure_vel[0] = GyroInput[3 * measure_imu] * DEG_TO_RAD;
            measure_vel[1] = GyroInput[3 * measure_imu + 1] * DEG_TO_RAD;
            measure_vel[2] = GyroInput[3 * measure_imu + 2] * DEG_TO_RAD;*/

            for (int i = 0; i < 3; i++) {
                base_vel[i] = FilteredDerivative(base_pos_prev[i], base_pos[i], base_vel_prev[i], 2 * PI * 2, T_S);
                measure_vel[i] = FilteredDerivative(measure_pos_prev[i], measure_pos[i], measure_vel_prev[i], 2 * PI * 2, T_S);
            }

            for (int i = 0; i < 3; i++) {
                base_acc_prev[i] = base_acc[i];
                measure_acc_prev[i] = measure_acc[i];
            }

            /*base_acc[0] = AccInput[3 * base_imu];
            base_acc[1] = AccInput[3 * base_imu + 1];
            base_acc[2] = AccInput[3 * base_imu + 2];
            measure_acc[0] = AccInput[3 * measure_imu];
            measure_acc[1] = AccInput[3 * measure_imu + 1];
            measure_acc[2] = AccInput[3 * measure_imu + 2];*/
            for (int i = 0; i < 3; i++) {
                base_acc[i] = FilteredDerivative(base_vel_prev[i], base_vel[i], base_acc_prev[i], 2 * PI * 100, T_S);
                measure_acc[i] = FilteredDerivative(measure_vel_prev[i], measure_vel[i], measure_acc_prev[i], 2 * PI * 100, T_S);
            }

            base_sEmg[0] = EMGInput[3 * base_imu];
            base_sEmg[1] = EMGInput[3 * base_imu + 1];
            base_sEmg[2] = EMGInput[3 * base_imu + 2];
            measure_sEmg[0] = EMGInput[3 * measure_imu];
            measure_sEmg[1] = EMGInput[3 * measure_imu + 1];
            measure_sEmg[2] = EMGInput[3 * measure_imu + 2];
             
            /*std::system("cls");
            std::cout << std::showpoint << std::setprecision(6) << std::endl;
            std::cout << "measure deg: " << EulerInput[3 * measure_imu] << ", " << "base deg: " << EulerInput[3 * base_imu] << std::endl;
            std::cout << "measure deg: " << EulerInput[3 * measure_imu + 1] << ", " << "base deg: " << EulerInput[3 * base_imu + 1] << std::endl;
            std::cout << "measure deg: " << EulerInput[3 * measure_imu + 2] << ", " << "base deg: " << EulerInput[3 * base_imu + 2] << std::endl;
            std::cout << "measure vel: " << GyroInput[3 * measure_imu] << ", " << "base vel: " << GyroInput[3 * base_imu] << std::endl;
            std::cout << "measure vel: " << GyroInput[3 * measure_imu + 1] << ", " << "base vel: " << GyroInput[3 * base_imu + 1] << std::endl;
            std::cout << "measure vel: " << GyroInput[3 * measure_imu + 2] << ", " << "base vel: " << GyroInput[3 * base_imu + 2] << std::endl;
            std::cout << "EMG: " << EMGInput[measure_imu * 3] << std::endl;
            std::cout << "EMG: " << EMGInput[measure_imu * 3 + 1] << std::endl;
            std::cout << "EMG: " << EMGInput[measure_imu * 3 + 2] << std::endl;*/
        }


        //Plotting
        if (pos_online) {
            t_p += ImGui::GetIO().DeltaTime;
            Pos_buf[0].AddPoint(t_p, Left);
            Pos_buf[1].AddPoint(t_p, Right);
            //Vel_buf[0].AddPoint(t_p, ReadData.VelAct_L);                // 얘네 쓰면 velocity 값을 센서에서 받아오는 값을 사용함.
            //Vel_buf[1].AddPoint(t_p, ReadData.VelAct_R);
            Vel_buf[0].AddPoint(t_p, vel_cal_L);              //얘네 쓰면 velocity 값을 센서 값이 아니라 위치의 미분을 사용함. 근데 왜인지 모르게 오히려 노이즈가 더 생겨서 그냥 센서 값 쓸 예정
            Vel_buf[1].AddPoint(t_p, vel_cal_R);
            Acc_buf[0].AddPoint(t_p, Acc_cal_L);
            Acc_buf[1].AddPoint(t_p, Acc_cal_R);
            Tor_buf.AddPoint(t_p, torque_filtered);
            if (semg_mode == RAW) {
                sEMG_buf[0].AddPoint(t_p, sEMG_raw_ch1);
                sEMG_buf[1].AddPoint(t_p, sEMG_raw_ch2);
            }
            else if (semg_mode == ABS) {
                sEMG_buf[0].AddPoint(t_p, sEMG_abs_ch1);
                sEMG_buf[1].AddPoint(t_p, sEMG_abs_ch2);
            }
            else {
                sEMG_buf[0].AddPoint(t_p, sEMG_MAV_Fin_ch1);
                sEMG_buf[1].AddPoint(t_p, sEMG_MAV_Fin_ch2);
            }
            for (int i = 0; i < 3; i++) {
                IMU_Pos_buf[i].AddPoint(t_p, measure_pos[i]);
                IMU_Vel_buf[i].AddPoint(t_p, measure_vel[i]);
                IMU_Acc_buf[i].AddPoint(t_p, measure_acc[i]);
                IMU_sEMG_buf[i].AddPoint(t_p, measure_sEmg[i]);
            }
        }


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Patient Window");
            ImGui::Indent();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Now it's "); ImGui::SameLine();
            if(ImGui::Button(rehab_mode == passive ? "Passive" : "Active", ImVec2(80, 0))){
                if (rehab_mode == passive) {
                    rehab_mode = active;
                }
                else {
                    rehab_mode = passive;
                }
            } ImGui::SameLine();
            ImGui::Text("mode, Click to switch mode");
            ImGui::Unindent();

            if (ImGui::Button(pos_online ? "Stop" : "Start")) {
                pos_online = !pos_online;
            } ImGui::SameLine();
            if (ImGui::Button(btn_minmax_set ? "Stop" : "min/Max set")) {
                ImGui::OpenPopup("Set min/Max");
                isMinSet = false;
                isMaxSet = false;
            }

            if (ImGui::BeginPopupModal("Set min/Max", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {                if (!isMinSet) {
                    ImGui::Text("Press OK after make your arm as low as possible.\n");
                    if (ImGui::Button("OK")) { isMinSet = true; }
                    if (ReadData.PosInc_R < minPos) {
                        minPos = ReadData.PosInc_R;
                    }
                }
                else {
                    if (!isMaxSet) {
                        ImGui::Text("Press OK after make your arm as high as possible.\nif you want to reset lowest position, press prev.\n");
                        if (ImGui::Button("OK")) { 
                            isMaxSet = true;
                            PosOff_R = minPos;
                            range = (maxPos - minPos);
                            ImGui::CloseCurrentPopup();
                        } ImGui::SameLine();
                        if (ImGui::Button("Prev")) { isMinSet = false; }
                        if (ReadData.PosInc_R > maxPos) {
                            maxPos = ReadData.PosInc_R;
                        }
                    }
                }
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));

            ImGui::VSliderFloat("Limb", ImVec2(50, 200), &Left, -0.1 * range, 1.1 * range, ""); ImGui::SameLine();
            ImGui::VSliderFloat("Exo", ImVec2(50, 200), &Right, -0.1 * range, 1.1 * range, ""); ImGui::SameLine();
            ImGui::VSliderFloat("desired", ImVec2(50, 200), &desired, -0.1 * range, 1.1 * range, ""); ImGui::SameLine();
            if (rehab_mode == active) {
                ImGui::VSliderInt("Resistance", ImVec2(30, 200), &torque, -10, 10);
            }
            else {
                ImGui::VSliderInt("Speed", ImVec2(30, 200), &speed, 0, 10);
            }
            if (ImGui::Button("Reset", ImVec2(50, 30)))
                PosRst = true;

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::PopStyleVar(3);
            if (ImGui::Button(show_experimenter_window ? "CLOSE EXPERIMENTER WINDOW" : "OPEN EXPERIMENTER WINDOW", ImVec2(200, 30))) {
                show_experimenter_window = !show_experimenter_window;
            } ImGui::SameLine();

            if (ImGui::Button(show_imu_window ? "CLOSE IMU WINDOW" : "OPEN IMU WINDOW", ImVec2(200, 30))) {
                if (show_imu_window) {
                    //CloseMatch();
                    show_imu_window = false;
                }
                else {
                    /*InitMatch();
                    OpenMatch();*/
                    show_imu_window = true;
                }
            }


            ImGui::End();
        }

        // 3. Show Experimenter window.
        if (show_experimenter_window)
        {
            ImGui::Begin("Experimenter Window", &show_experimenter_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

            if (ImGui::Button(pos_online ? "Stop" : "Start"))
                pos_online = !pos_online;
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
                exp_rst = true;

            if (ImPlot::BeginSubplots("Experimenter Plots", 3, 2, ImVec2(-1, 900), ImPlotSubplotFlags_LinkAllX)) {

                if (ImPlot::BeginPlot("Position")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1 * range, 1.1 * range);
                    for (int i = 0; i < 2; ++i) {
                        snprintf(label, sizeof(label), i ? "Exo" : "Limb");
                        if (Pos_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &Pos_buf[i].Data[0].x, &Pos_buf[i].Data[0].y, Pos_buf[i].Data.size(), 0, Pos_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("Torque")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -25, 25);
                    snprintf(label, sizeof(label), "Torque");
                    if (Tor_buf.Data.size() > 0)
                        ImPlot::PlotLine(label, &Tor_buf.Data[0].x, &Tor_buf.Data[0].y, Tor_buf.Data.size(), 0, Tor_buf.Offset, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("Velocity")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -10, 10);
                    for (int i = 0; i < 2; ++i) {
                        snprintf(label, sizeof(label), i ? "Exo" : "Limb");
                        if (Vel_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &Vel_buf[i].Data[0].x, &Vel_buf[i].Data[0].y, Vel_buf[i].Data.size(), 0, Vel_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("sEMG")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -2, 2);
                    for (int i = 0; i < 2; ++i) {
                        snprintf(label, sizeof(label), "Ch.%d", (i + 1));
                        if (Vel_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &sEMG_buf[i].Data[0].x, &sEMG_buf[i].Data[0].y, sEMG_buf[i].Data.size(), 0, sEMG_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("Acceleration")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -500, 500);
                    for (int i = 0; i < 2; ++i) {
                        snprintf(label, sizeof(label), i ? "Exo" : "Limb");
                        if (Vel_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &Acc_buf[i].Data[0].x, &Acc_buf[i].Data[0].y, Acc_buf[i].Data.size(), 0, Acc_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }




            }
            ImPlot::EndSubplots();
            if (ImGui::RadioButton("Raw", semg_mode == RAW)) { semg_mode = RAW; } ImGui::SameLine();
            if (ImGui::RadioButton("ABS", semg_mode == ABS)) { semg_mode = ABS; } ImGui::SameLine();
            if (ImGui::RadioButton("MAV", semg_mode == MAV)) { semg_mode = MAV; }
            if (ImGui::Button("Save")) {
                now = time(NULL);
                ptm = localtime(&now);
                strftime(timebuf, 64, "_%Y%m%d_%H%M%S", ptm);
                SaveFolderPath = "C:/Users/EXP/Desktop/";
                SaveFolderName = "SaveData";
                SaveFolderName += timebuf;
                SaveFolderPath += SaveFolderName;
                static std::wstring w_str;
                w_str = std::wstring(SaveFolderPath.begin(), SaveFolderPath.end());
                static LPCWSTR SaveFolderPath_;
                SaveFolderPath_ = w_str.c_str();
                std::cout << SaveFolderPath << std::endl;

                CreateDirectory(SaveFolderPath_, NULL);

                static std::ofstream of_pos_l;
                static std::ofstream of_pos_r;
                static std::ofstream of_vel_l;
                static std::ofstream of_vel_r;
                static std::ofstream of_acc_l;
                static std::ofstream of_acc_r;
                static std::ofstream of_torque;
                static std::ofstream of_emg_ch1;
                static std::ofstream of_emg_ch2;
                static std::ofstream of_imupos;
                static std::ofstream of_imuvel;
                static std::ofstream of_imuacc;
                static std::ofstream of_imuemg;

                of_pos_l.open(SaveFolderPath + "/MotorPosL.txt");
                of_pos_r.open(SaveFolderPath + "/MotorPosR.txt");
                of_vel_l.open(SaveFolderPath + "/MotorVelL.txt");
                of_vel_r.open(SaveFolderPath + "/MotorVelR.txt");
                of_acc_l.open(SaveFolderPath + "/MotorAccL.txt");
                of_acc_r.open(SaveFolderPath + "/MotorAccR.txt");
                of_torque.open(SaveFolderPath + "/MotorTorque.txt");
                of_emg_ch1.open(SaveFolderPath + "/SensorEmgCh1.txt");
                of_emg_ch2.open(SaveFolderPath + "/SensorEmgCh2.txt");
                of_imupos.open(SaveFolderPath + "/ImuPos.txt");
                of_imuvel.open(SaveFolderPath + "/ImuVel.txt");
                of_imuacc.open(SaveFolderPath + "/ImuAcc.txt");
                of_imuemg.open(SaveFolderPath + "/ImuEmg.txt");

                for (int i = 0; i < Pos_buf[0].Data.Size; i++) {
                    of_pos_l << Pos_buf[0].Data.Data[i].x << " " << Pos_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Pos_buf[1].Data.Size; i++) {
                    of_pos_r << Pos_buf[1].Data.Data[i].x << " " << Pos_buf[1].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Vel_buf[0].Data.Size; i++) {
                    of_vel_l << Vel_buf[0].Data.Data[i].x << " " << Vel_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Vel_buf[1].Data.Size; i++) {
                    of_vel_r << Vel_buf[1].Data.Data[i].x << " " << Vel_buf[1].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Acc_buf[0].Data.Size; i++) {
                    of_acc_l << Acc_buf[0].Data.Data[i].x << " " << Acc_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Acc_buf[1].Data.Size; i++) {
                    of_acc_r << Acc_buf[1].Data.Data[i].x << " " << Acc_buf[1].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < Tor_buf.Data.Size; i++) {
                    of_torque << Tor_buf.Data.Data[i].x << " " << Tor_buf.Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < sEMG_buf[0].Data.Size; i++) {
                    of_emg_ch1 << sEMG_buf[0].Data.Data[i].x << " " << sEMG_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < sEMG_buf[1].Data.Size; i++) {
                    of_emg_ch2 << sEMG_buf[1].Data.Data[i].x << " " << sEMG_buf[1].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < IMU_Pos_buf[0].Data.Size; i++) {
                    of_imupos << IMU_Pos_buf[0].Data.Data[i].x << " " << IMU_Pos_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < IMU_Vel_buf[0].Data.Size; i++) {
                    of_imuvel << IMU_Vel_buf[0].Data.Data[i].x << " " << IMU_Vel_buf[0].Data.Data[i].y << std::endl;
                }
                for (int i = 0; i < IMU_Acc_buf[0].Data.Size; i++) {
                    of_imuacc << IMU_Acc_buf[0].Data.Data[i].x << " " << IMU_Acc_buf[0].Data.Data[i].y << std::endl;
                }
            }
            static char inputpath[128] = "C:/Users/EXP/Desktop/";
            ImGui::InputText("Input the directory path", inputpath, IM_ARRAYSIZE(inputpath));
            ImGui::SameLine();

            if (ImGui::Button("Load")) {
                
            }      

            ImGui::End();
        }

        //4. IMU Window
        if (show_imu_window) {
            ImGui::Begin("IMU Window", &show_imu_window);
            ImGui::InputInt("Measure Sensor ID", &measure_imu);
            ImGui::InputInt("Base Sensor ID", &base_imu);
            if (ImPlot::BeginSubplots("IMU Plots", 2, 2, ImVec2(-1, 600), ImPlotSubplotFlags_LinkAllX)) {
                if (ImPlot::BeginPlot("IMU POS")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -4, 4);
                    for (int i = 0; i < 3; ++i) {
                        snprintf(label, sizeof(label), "Pos %d", i); 
                        if (IMU_Pos_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &IMU_Pos_buf[i].Data[0].x, &IMU_Pos_buf[i].Data[0].y, IMU_Pos_buf[i].Data.size(), 0, IMU_Pos_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }
                if (ImPlot::BeginPlot("IMU vel")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -10, 10);
                    for (int i = 0; i < 3; ++i) {
                        snprintf(label, sizeof(label), "Vel %d", i);
                        if (IMU_Vel_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &IMU_Vel_buf[i].Data[0].x, &IMU_Vel_buf[i].Data[0].y, IMU_Vel_buf[i].Data.size(), 0, IMU_Vel_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }
                if (ImPlot::BeginPlot("IMU Acceleration")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -5000, 5000);
                    for (int i = 0; i < 3; ++i) {
                        snprintf(label, sizeof(label), "Acc % d", i);
                        if (IMU_Acc_buf[i].Data.size() > 0)
                            ImPlot::PlotLine(label, &IMU_Acc_buf[i].Data[0].x, &IMU_Acc_buf[i].Data[0].y, IMU_Acc_buf[i].Data.size(), 0, IMU_Acc_buf[i].Offset, 2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }
                if (ImPlot::BeginPlot("IMU sEMG")) {
                    ImPlot::SetupAxisLimits(ImAxis_X1, t_p - 7.0, t_p, !pos_online ? ImGuiCond_Once : ImGuiCond_Always);
                        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 4);
                        for (int i = 0; i < 3; ++i) {
                            snprintf(label, sizeof(label), "sEMG %d", i);
                                if (IMU_sEMG_buf[i].Data.size() > 0)
                                    ImPlot::PlotLine(label, &IMU_sEMG_buf[i].Data[0].x, &IMU_sEMG_buf[i].Data[0].y, IMU_sEMG_buf[i].Data.size(), 0, IMU_sEMG_buf[i].Offset, 2 * sizeof(float));
                        }
                    ImPlot::EndPlot();
                }
                ImPlot::EndSubplots();
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    CloseMatch();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();


    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
