/*
 * AnimationUtils.h
 *
 *  Created on: Oct 14, 2019
 *      Author: rey
 */

#ifndef SRC_HEADERS_ANIMATIONUTILS_H_
#define SRC_HEADERS_ANIMATIONUTILS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string matToString(glm::mat4 matrix) {
	std::stringstream ss;
	const float *pSource = (const float*)glm::value_ptr(matrix);
	for (int i = 0; i < 16; ++i) {
		ss << pSource[i];
		if(i < 15)
			ss << ",";
	}
	return ss.str();
}

void appendFrame(std::ofstream &outputFile,
		std::vector<glm::mat4> matrixList){
	std::stringstream ss;
	for(unsigned int i = 0; i < matrixList.size(); i++)
		ss << matToString(matrixList[i]) << "|";
	outputFile << ss.str() << "|" << std::endl;
}

std::vector<std::vector<glm::mat4>> getKeyFrames(std::string fileName) {
	std::vector<std::vector<glm::mat4>> keyFrames;
	std::string line;
	std::ifstream infile(fileName);
	std::string s = line;
	while (std::getline(infile, line))
	{
		std::vector<glm::mat4> transforms;
		s = line;
		size_t pos1 = 0;
		std::string token1;
		std::string delimiter1 = "|";
		while ((pos1 = s.find(delimiter1)) != std::string::npos) {
			token1 = s.substr(0, pos1);
			//std::cout << token1 << std::endl;

			size_t pos2 = 0;
			std::string token2;
			std::string delimiter2 = ",";

			int i = 0;
			int j = 0;
			glm::mat4 transform;
			float aaa[16];
			while ((pos2 = token1.find(delimiter2)) != std::string::npos) {
				token2 = token1.substr(0, pos2);
				aaa[i++] = atof(token2.c_str());
				/*if (j == 0)
					transform[i].x = atof(token2.c_str());
				if (j == 1)
					transform[i].y = atof(token2.c_str());
				if (j == 2)
					transform[i].z = atof(token2.c_str());
				if (j == 3)
					transform[i].w = atof(token2.c_str());*/
				/*j++;
				if (j > 3) {
					i++;
					j = 0;
				}*/
				token1.erase(0, pos2 + delimiter2.length());
			}
			aaa[i++] = atof(token1.c_str());
			transform = glm::make_mat4(aaa);
			transforms.push_back(transform);
			s.erase(0, pos1 + delimiter1.length());
		}
		keyFrames.push_back(transforms);
	}
	return keyFrames;
}

glm::mat4 interpolate(std::vector<std::vector<glm::mat4>> keyFrames,
		int indexNext, int jointID, float interpolation){
	glm::quat firstQuat;
	glm::quat secondQuat;
	glm::quat finalQuat;
	glm::mat4 interpoltaedMatrix;
	glm::vec4 transformComp1;
	glm::vec4 transformComp2;
	glm::vec4 finalTrans;

	firstQuat = glm::quat_cast(keyFrames[indexNext - 1][jointID]);
	secondQuat = glm::quat_cast(keyFrames[indexNext][jointID]);
	finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
	interpoltaedMatrix = glm::mat4_cast(finalQuat);
	transformComp1 = keyFrames[indexNext - 1][jointID] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	transformComp2 = keyFrames[indexNext][jointID] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
	interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;
	return interpoltaedMatrix;
}

#endif /* SRC_HEADERS_ANIMATIONUTILS_H_ */
