	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &EBO2);
	glGenBuffers(1, &VBO2);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2),vertices2, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2),indices2, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// EBO2 as the vertex attribute's bound vertex buffer object so afterwards we
	// can safely unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

	// You can unbind the VAO2 afterwards so other VAO2 calls won't accidentally
	// modify this VAO2, but this rarely happens. Modifying other VAO2s requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAO2s (nor
	// EBO2s) when it's not directly necessary.
	glBindVertexArray(0);