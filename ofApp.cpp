#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(24);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face_mesh.clear();
	this->frame_mesh.clear();

	auto r = 15;
	auto v_span = 4;
	auto u_span = 8;
	auto v_start = ofGetFrameNum();

	for (auto R = 50; R <= 250; R += 100) {

		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		for (auto v = 0; v < 360; v += v_span) {

			for (auto u = 0; u < 360; u += u_span) {

				vector<glm::vec3> vertices;
				vertices.push_back(this->make_point(R, r, u, v));
				vertices.push_back(this->make_point(R, r, u + u_span, v));
				vertices.push_back(this->make_point(R, r, u + u_span, v + v_span));
				vertices.push_back(this->make_point(R, r, u, v + v_span));

				for (auto& vertex : vertices) {

					auto angle_x = ofMap(ofNoise(noise_seed.x + vertex.x * 0.0005, ofGetFrameNum() * 0.005), 0, 1, -PI, PI);
					auto angle_y = ofMap(ofNoise(noise_seed.y + vertex.y * 0.0005, ofGetFrameNum() * 0.005), 0, 1, -PI, PI);
					auto angle_z = ofMap(ofNoise(noise_seed.z + vertex.z * 0.0005, ofGetFrameNum() * 0.005), 0, 1, -PI, PI);

					auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
					auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
					auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

					vertex = glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
				}

				int index = this->face_mesh.getNumVertices();
				this->face_mesh.addVertices(vertices);
				this->frame_mesh.addVertices(vertices);

				this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 1); this->face_mesh.addIndex(index + 2);
				this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 3);

				this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 1);
				this->frame_mesh.addIndex(index + 1); this->frame_mesh.addIndex(index + 2);
				this->frame_mesh.addIndex(index + 2); this->frame_mesh.addIndex(index + 3);
				this->frame_mesh.addIndex(index + 3); this->frame_mesh.addIndex(index + 0);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	ofSetColor(239);
	this->face_mesh.draw();

	ofSetColor(30);
	this->frame_mesh.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);
	z += ofMap(ofNoise(cos(v), sin(v), ofGetFrameNum() * 0.005), 0, 1, -50, 50);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}