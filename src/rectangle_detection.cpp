#include <Eigen/Dense>
#include <array>
#include <vector>

namespace rectangle_detect {
/*************************************************************************************/
std::vector<std::array<float, 4>> find_pairs(const std::vector<float>& rho_maxs, const std::vector<float>& theta_maxs,
                                             const float& T_rho, const float& T_t, const float& T_L) {
    // Match peaks into pairs
    std::vector<std::array<float, 4>> pairs;  // 1st: rho, 2nd: theta
    std::array<float, 4> pair;
    for (int i = 0; i < rho_maxs.size(); ++i) {
        for (int j = 0; j < rho_maxs.size(); ++j) {
            // Remove lines too close to origin (remove when windowed Hough is implemented)
            if (rho_maxs[i] < 3) continue;

            // Parralelism
            if (abs(theta_maxs[i] - theta_maxs[j]) > T_t) continue;

            // Symmetry wrt x axis
            if (abs(rho_maxs[i] + rho_maxs[j]) > T_rho) continue;

            // Approximately same length

            // Construct extended peak
            pair[0] = 0.5 * abs(rho_maxs[i] - rho_maxs[j]);
            pair[1] = 0.5 * (theta_maxs[i] + theta_maxs[j]);
            pair[2] = abs(rho_maxs[i] + rho_maxs[j]);      // error measure on rho
            pair[3] = abs(theta_maxs[i] - theta_maxs[j]);  // error measure on theta

            pairs.push_back(pair);
        }
    }

    return pairs;
}

/*************************************************************************************/
std::vector<std::array<float, 8>> match_pairs_into_rectangle(const std::vector<std::array<float, 4>>& pairs,
                                                             const float& T_alpha) {
    std::vector<std::array<float, 8>> rectangles;

    // Match pairs into rectangle
    for (int i = 0; i < pairs.size(); i++) {
        for (int j = 0; j < pairs.size(); j++) {
            if (j == i) continue;

            // Orthogonality
            float delta_alpha = abs(abs(pairs[i][1] - pairs[j][1]) - 90);
            if (delta_alpha > T_alpha) continue;

            rectangles.push_back({pairs[i][1], pairs[i][0], pairs[j][0], pairs[i][2], pairs[j][2], pairs[i][3],
                                  pairs[j][3], delta_alpha});
        }
    }

    return rectangles;
}

/*************************************************************************************/
std::array<int, 8> remove_duplicates(std::vector<std::array<int, 8>> rectangles, float a, float b) {
    float criteria =
        sqrt(a * (rectangles[0][5] + rectangles[0][6] + rectangles[0][7]) + b * (rectangles[0][3] + rectangles[0][4]));
    float new_criteria;
    std::array<int, 8> rect;
    rect = rectangles[0];

    for (int i = 1; i < rectangles.size(); ++i) {
        new_criteria = sqrt(a * (rectangles[i][5] + rectangles[i][6] + rectangles[i][7]) +
                            b * (rectangles[i][3] + rectangles[i][4]));
        if (new_criteria < criteria) {
            criteria = new_criteria;
            // std::cout << criteria << std::endl;
            rect = rectangles[i];
        }
    }

    return rect;
}

/*************************************************************************************/
std::array<float, 8> remove_duplicates(std::vector<std::array<float, 8>> rectangles, float a, float b) {
    float criteria =
        sqrt(a * (rectangles[0][5] + rectangles[0][6] + rectangles[0][7]) + b * (rectangles[0][3] + rectangles[0][4]));
    float new_criteria;
    std::array<float, 8> rect;
    rect = rectangles[0];

    for (int i = 1; i < rectangles.size(); ++i) {
        new_criteria = sqrt(a * (rectangles[i][5] + rectangles[i][6] + rectangles[i][7]) +
                            b * (rectangles[i][3] + rectangles[i][4]));
        if (new_criteria < criteria) {
            criteria = new_criteria;
            // std::cout << criteria << std::endl;
            rect = rectangles[i];
        }
    }

    return rect;
}
}
