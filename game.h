#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
*/

#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "asteroid.h"
#include "streetlamp.h"
#include "branch.h"
#include "cig.h"
//#include "audio_manager.h"

namespace game {

    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            // Call Init() before calling any other method
            void Init(void); 
            // Set up resources for the game
            void SetupResources(void);
            // Set up initial scene
            void SetupScene(void);
            // Run the game: keep the application active
            void MainLoop(void); 

        private:
            // GLFW window
            GLFWwindow* window_;

            // Scene graph containing all nodes to render
            SceneGraph scene_;
            std::vector<Streetlamp*> streetlamps_;

            // Resources available to the game
            ResourceManager resman_;

            // Camera abstraction
            Camera camera_;

            // Flag to turn animation on/off
            bool animating_;

            // Flag to turn visual blur on/off
            bool blur_;

            // Flag to turn noir filter on/off
            bool noir_;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
 
            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            static void CursorCallback(GLFWwindow* window, double xPos, double yPos);
            double oldMouseX, oldMouseY; //helper vals for cursorCallback

            // Create an instance of a streetlamp (includes the position of its directional light)
            Streetlamp* Game::CreateStreetlampInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name);

            // Creates a street environment at a location based on a template
            Cigarette* Game::CreateCigaretteInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name);
            //static Cigarette* Cig;


            void CreateRoad(int num_roads = 5);
            //give the initial position of the buildings and how much room you have for them
            void CreateBuildings(glm::vec3 initPos, float room);

            // Tree
            // Create instance of a branch
            Branch* CreateBranchInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, Branch* parent_branch, int depth, int id_in_set, int branch_per_level);
            // Create an entire tree
            void CreateTree(int branch_per_level = 5, glm::vec3 position = glm::vec3(0, 0, 0));

            // Create an instance of an object stored in the resource manager
            SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));


            //Audio Manager Class
            /*AudioManager am;*/
            int rainIndex;


    }; // class Game

} // namespace game

#endif // GAME_H_
