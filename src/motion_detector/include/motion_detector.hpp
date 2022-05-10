#include <vector>
#include <random>

#include <QDebug>

using namespace std;

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
        mock_motion.clear();

        index = mFPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = mFPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);

        index = mFPS * 10;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = mFPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);

        const auto range_from = 0;
        const auto range_to = static_cast<int>(mock_motion.size());
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<int> distr(range_from, range_to);
        index = distr(generator);
    }

    Motion detect(const A & previous_frame, const A & current_frame){
        ++index;
        return mock_motion[index%mock_motion.size()];
    }

    void setFps (int fps)
    {
        assert(fps > 0);
        this->mFPS = fps;
    }

private:
    std::vector<Motion> mock_motion;
    std::uint64_t index = 0;
    int mFPS = 25;
};
