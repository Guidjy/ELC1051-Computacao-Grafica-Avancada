using System;
using System.IO;
using GLFW;
using static OpenGL.Gl;
using GlmNet;
using StbImageSharp;

//https://github.com/ForeverZer0/glfw-net

namespace HelloTriangle
{
    class Program
    {
        private const string TITLE = "Hello Triangle!";

        static uint []vaoID;


        static bool pressed = false;
        static bool released = true;
        static vec2 oldMouse = new vec2();
        static bool keyPressedP = false;

        //ver https://github.com/ForeverZer0/glfw-net/issues/36 para gerenciamento de callbacks.
        static private MouseCallback cursorPositionCallback;
        static private KeyCallback keyCallback;

        static void Main(string[] args)
        {
            // Set context creation hints
            PrepareContext();
            // Create a window and shader program
            Window window = CreateWindow(900, 700);
            var program = CreateProgram();

            CreateVertices();

            BindCallbacks(window);


            uint []tex = glGenTextures(1);
            
            glBindTexture(GL_TEXTURE_2D, tex[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //STB_IMAGE:  https://github.com/StbSharp/StbImageSharp/blob/master/README.md
            Stream stream = File.OpenRead("./imagem.png");
            ImageResult image = ImageResult.FromStream(stream, ColorComponents.RedGreenBlueAlpha);
            unsafe
            {
                fixed (void* v = &image.Data[0])
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width, image.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, v);
                }
            }

            //glEnable(GL_TEXTURE_2D);
            //glEnable(GL_TEXTURE);
            //glActiveTexture(GL_TEXTURE0);
            /**/

            int projectionMatrix = glGetUniformLocation(program, "projectionMatrix");
            int modelMatrix = glGetUniformLocation(program, "modelMatrix");
            Console.WriteLine("Proj: " + projectionMatrix + "  model: " + modelMatrix);


            //SetCallBacks(window);

            glClearColor(1, 1, 1, 0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);


            while (!Glfw.WindowShouldClose(window))
            {
                // Swap fore/back framebuffers, and poll for operating system events.
                Glfw.SwapBuffers(window);
                Glfw.PollEvents();


                // Clear the framebuffer to defined background color
                glClear(GL_COLOR_BUFFER_BIT);

                glLineWidth(4);

                //HandleMouseEvents(window);
                //HandleKeyboardEvents(window);



                int w = 100, h = 100;
                Glfw.GetFramebufferSize(window, out w, out h);
                //Console.WriteLine("w: " + w + "  h: " + h);

                //glViewport(0, 0, w, h);
                 mat4 mat;
                 mat = glm.ortho(0, w, h, 0);
                 glUniformMatrix4fv(projectionMatrix, 1, false, mat.to_array());


                 uint[] indices = new uint[6]
                     {
                         0, 1, 2,
                         0, 2, 3
                     };


                 mat4 model = new mat4(1);
                 glUniformMatrix4fv(modelMatrix, 1, false, model.to_array());

                 glBindVertexArray(vaoID[0]);
                 glDrawElements(GL_TRIANGLES, indices); //FUNCIONA, soh nao se pode criar o GL_ELEMENT_ARRAY_BUFFER, senao da pau
                 //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Funciona
                 glBindVertexArray(0);

                 glm.translate(model, new vec3(100, 100, 0));
                 glUniformMatrix4fv(modelMatrix, 1, false, model.to_array());
                 glBindVertexArray(vaoID[0]);
                 glDrawElements(GL_TRIANGLES, indices); //FUNCIONA, soh nao se pode criar o GL_ELEMENT_ARRAY_BUFFER, senao da pau
                 //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Funciona
                 glBindVertexArray(0);
                 /**/


            }

            Glfw.Terminate();
        }


        public static void OnMouseMove(double x, double y)
        {
            Console.WriteLine("Cursor: " + x);
        }

        public static void OnKey(Keys key, int code, InputState state, ModifierKeys mod)
        {
            Console.WriteLine("Key: " + key + " code:" + code + " State: " + state + " Modif: "+ mod);
        }
        private static void BindCallbacks(Window window)
        {
            cursorPositionCallback = (_, x, y) => OnMouseMove(x, y);
            keyCallback = (_, key, code, state, mods) => OnKey(key, code, state, mods);

            Glfw.SetCursorPositionCallback(window, cursorPositionCallback);
            Glfw.SetKeyCallback(window, keyCallback);
        }


        private static void HandleKeyboardEvents(Window window)
        {
            InputState st = Glfw.GetKey(window, Keys.P);
            if(st == InputState.Press && keyPressedP == false)
            {
                keyPressedP = true;
                Console.WriteLine("press P: ");
            }
            else if (st == InputState.Release && keyPressedP == true)
            {
                keyPressedP = false;
                Console.WriteLine("Release P: ");
            }
        }

        private static void HandleMouseEvents(Window window)
        {
            double x, y;
            Glfw.GetCursorPosition(window, out x, out y);
            InputState st = Glfw.GetMouseButton(window, MouseButton.Button1);
            //PRESS - CLICK
            if (st == InputState.Press && pressed == false)
            {
                pressed = true;
                released = false;
                oldMouse.x = (float)x;
                oldMouse.y = (float)y;
                Console.WriteLine("press: " + x + " " + y);
                TrataCliqueBotao();
            }
            //DRAG
            if (st == InputState.Press && pressed == true)
            {
                if ((float)x != oldMouse.x && (float)y != oldMouse.y)
                {
                    oldMouse.x = (float)x;
                    oldMouse.y = (float)y;
                    Console.WriteLine("Drag: " + x + " " + y);
                }

                //TrataCliqueBotao();
            }
            //RELEASE - DROP
            if (st == InputState.Release && released == false)
            {
                Console.WriteLine("Release: " + x + " " + y);
                released = true;
                pressed = false;
            }
            //MOVING
            if (st == InputState.Release && released == true)
            {
                if ((float)x != oldMouse.x && (float)y != oldMouse.y)
                {
                    oldMouse.x = (float)x;
                    oldMouse.y = (float)y;
                    Console.WriteLine("Moving: " + x + " " + y);
                }
            }

        }

        private static void TrataCliqueBotao()
        {

        }

        //Estao gerando erro no GLFW. 
        private static void SetCallBacks(Window window)
        {
            //Glfw.SetErrorCallback(errorHandler);
            //Glfw.SetCursorPositionCallback(window, mouseHandler);  //MouseCallback
            //Glfw.SetFramebufferSizeCallback(window, resizeHandler); //SizeCallback
            //Glfw.SetDropCallback(window, dropHandler); // FileDropCallback
        }

        private static void errorHandler(ErrorCode error, IntPtr message)
        {
            Console.WriteLine("ERRO" + message.ToString() );
            //MessageBox.Show(description, "Error: Glfw is not initalized.");
        }

        private static void mouseHandler(Window win, double x, double y)
        {
            Console.WriteLine("Mouse: " + x + "  " + y);
        }

        private static void resizeHandler(Window win, int w, int h)
        {
            Console.WriteLine("Size: " + w + "  " + h);
        }

        //FileDropCallback
        private static void dropHandler(Window win, int cont, IntPtr message)
        {
            //string s = System.Runtime.InteropServices.Marshal.PtrToStringAuto(message);
            //Console.WriteLine("Drop: " + cont + "  " + s);
        }

        private static void PrepareContext()
        {
            // Set some common hints for the OpenGL profile creation
            Glfw.WindowHint(Hint.ClientApi, ClientApi.OpenGL);
            Glfw.WindowHint(Hint.ContextVersionMajor, 3); 
            Glfw.WindowHint(Hint.ContextVersionMinor, 3);
            Glfw.WindowHint(Hint.OpenglProfile, Profile.Core);
            Glfw.WindowHint(Hint.Doublebuffer, true);
            Glfw.WindowHint(Hint.Decorated, true);
        }

        private static Window CreateWindow(int width, int height)
        {
            // Create window, make the OpenGL context current on the thread, and import graphics functions
            var window = Glfw.CreateWindow(width, height, TITLE, Monitor.None, Window.None);
            Glfw.MakeContextCurrent(window);
            Import(Glfw.GetProcAddress);
            
            // Center window
            var screen = Glfw.PrimaryMonitor.WorkArea;
            var x = (screen.Width - width) / 2;
            var y = (screen.Height - height) / 2;
            Glfw.SetWindowPosition(window, x, y);
            //Glfw.SetWindowSize(window, 10, 10);

            return window;
        }
        
        private static uint CreateProgram()
        {
            var vertex = CreateShader(GL_VERTEX_SHADER, File.ReadAllText("./triangle.vert"));
            var fragment = CreateShader(GL_FRAGMENT_SHADER, File.ReadAllText("./triangle.frag"));
            
            var program = glCreateProgram();
            glAttachShader(program, vertex);
            glAttachShader(program, fragment);

            glLinkProgram(program);
            
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            glUseProgram(program);
            return program;
        }

        private static uint CreateShader(int type, string source)
        {
            var shader = glCreateShader(type);
            glShaderSource(shader, source);
            glCompileShader(shader);

            string s = glGetShaderInfoLog(shader);
            if (!string.IsNullOrWhiteSpace(s))
            {
                Console.WriteLine(s);
            }
 
            return shader;
        }

        private static unsafe void CreateVertices()
        {
            float w = 615;
            float h = 332;
            var vertices = new[] {
                0, 0,          0.0f,
                0, h,          0.0f,
                w, h,          0.0f,
                w, 0,          0.0f
            };

            var uv = new[]
            {
                0f, 0f,
                0f, 1f,
                1f, 1f,
                1f, 0f
            };

            vaoID = glGenVertexArrays(1);
            glBindVertexArray(vaoID[0]);

            uint[] vboID = glGenBuffers(2);

            //cria o VBO para vertices. 
            glBindBuffer(GL_ARRAY_BUFFER, vboID[0] );
            fixed (float* v = &vertices[0])
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.Length, v, GL_STATIC_DRAW);
            }
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
            glEnableVertexAttribArray(0);

            //cria o VBO para textura
            glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
            fixed (float* v = &uv[0])
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv.Length, v, GL_STATIC_DRAW);
            }
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(float), NULL);
            glEnableVertexAttribArray(1);


            //buffer de indices - NAO ESTA FUNCIONANDO.POR ISSO USO TRIANGLE_STRIP E GL_DRAW_ARRAYS
            /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[2]);
            fixed (uint* v = &indices[0])
            {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.Length, v, GL_STATIC_DRAW);
            }
            /**/
            glBindVertexArray(0);
        }
    }
}