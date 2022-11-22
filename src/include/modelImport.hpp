
struct rawModelStruct
{
    std::string name;
};
std::vector<rawModelStruct*> rawModels;

static inline void importModel(const std::string& name, const char* filePathC);
static inline rawModelStruct* getModel(std::string name);
static inline std::string getSlash();