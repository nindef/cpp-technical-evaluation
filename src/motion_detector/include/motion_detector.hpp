#include <vector>

using namespace std;

constexpr auto FPS = 25;

enum class Motion{
    MOTION,
    NO_MOTION
};

template<typename A>
class MockMotionDetector final
{
public:
    MockMotionDetector ()
    {
        initializeMotionVector();
    }

    void initializeMotionVector ()
    {
        auto index = FPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = FPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);

        index = FPS * 10;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = FPS * 10;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);

        index = std::rand() % mock_motion.size();
    }

    Motion detect(const A & previous_frame, const A & current_frame){
        ++index;
        return mock_motion[index%mock_motion.size()];
    }
private:
    std::vector<Motion> mock_motion;
    std::uint64_t index = 0;
};
