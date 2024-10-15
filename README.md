## O que é a GLSL?
GLSL (OpenGL Shading Language) é uma linguagem de programação de alto nível baseada na linguagem C, usada para escrever shaders que são executados na GPU. Foi criada pelo OpenGL ARB (OpenGL Architecture Review Board) para dar aos desenvolvedores mais controle direto sobre o pipeline gráfico.

No pipeline programável do OpenGL atual, dois tipos de shaders são obrigatórios:

1. **Vertex Shader**: Processa cada vértice separadamente. Ele é responsável por transformar as coordenadas dos vértices de um espaço de coordenadas para outro (por exemplo, de 3D para 2D) e pode também manipular atributos dos vértices, como posição, cor e vetores normais.
2. **Fragment Shader**: Uma vez que todos os vertex shaders processaram a posição final de todos os vértices, os fragment shaders rodam para cada fragmento entre os
vértices. O Fragment Shader processa cada fragmento (ou pixel potencial) gerado pelo rasterizador separadamente. Ele determina a cor final de cada pixel, aplicando operações como texturização, iluminação e outros efeitos visuais.

## O que são primitivas gráficas?

**Primitivas gráficas** são os elementos básicos usados para criar desenhos e representações visuais em computação gráfica. Eles incluem formas geométricas fundamentais como pontos, linhas, retêngulos, círculos, polígonos, etc.
Essas primitivas são usadas como bloco de contrução para criar formas e imagens mais complexas. Cada chamada de desenho (drawcall) é uma chamada à execução de um dos programas de shader da aplicação, atuando sobre um conjunto de primitivas gráficas (buffer)

As principais primitivas gráficas em OpenGL são:

- **GL_POINTS**: Cada vértice é tratado como um ponto individual.
- **GL_LINES**: Cada par de vértices define uma linha.
- **GL_TRIANGLES**: Cada trio de vértices define um triângulo.

## Como fazemos o armazenamento dos vértices na OpenGL?

Para o **armazenamento dos vértices** em OpenGL, utilizamos principalmente três tipos de buffers:
<details>
  <summary><h3>1. Vertex Buffer Object (VBO)</h3></summary>
  O VBO é um buffer usado no OpenGL para melhorar o desempenho do desenho de gráficos 3D. Ele permite que os dados de vértices (como posições, cores, vetores normais, coordenadas de textura, etc.) sejam armazenados diretamente na memória da GPU, prontos para serem usados pela pipeline de renderização, em vez de serem enviados repetidamente pela CPU a cada frame renderizado. Isso resulta em renderização mais eficiente e rápida, especialmente para cenas complexas com muitos vértices.

  ### Como utilizar?
  1. **Criação do VBO:** Cada buffer na OpenGL precisa ter um identificador único, para isso precisamos gerar este ID usando a função `glGenBuffers`. Assim, um buffer de memória é alocado na GPU:
     ```cpp
     GLuint vbo;
     glGenBuffers(1, &vbo);
     ```

  2. **Bind do VBO:** O buffer é "bindado", ou seja, vinculado ao contexto de OpenGL como um buffer de vértice. O OpenGL possui vários tipos de buffers. Os VBOs são do tipo `GL_ARRAY_BUFFER`. Devemos agora vincular o novo buffer criado:
     ```cpp
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     ```

  3. **Armazenamento de Dados:** Chamamos a função `glBufferData` que copia os dados dos vértices da memória da CPU para a memória da GPU.
     ```cpp
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
     ```
   
  4. **Desenho:** O OpenGL usa os dados armazenados no VBO para renderizar as primitivas.
     ```cpp
     glDrawArrays(GL_TRIANGLES, 0, 3);
     ```

  5. **Desvinculação e Exclusão (opcional):** O VBO pode ser desvinculado e deletado da GPU quando não for mais necessário.
     ```cpp
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glDeleteBuffers(1, &vbo);
     ```

  ### Tipos de Uso Comum
  - **GL_STATIC_DRAW:** Para dados que são definidos uma vez e usados muitas vezes (ex: modelos estáticos).
  - **GL_DYNAMIC_DRAW:** Para dados que serão alterados frequentemente (ex: modelos animados).
  - **GL_STREAM_DRAW:** Para dados que serão alterados em quase todos os frames (ex: partículas).
</details>
<details>
  <summary><h3>2. Vertex Array Object (VAO)</h3></summary>
  
  O **Vertex Array Object (VAO)** é um objeto no OpenGL que armazena o **estado de configuração dos atributos de vértice**. Ele simplifica a configuração e reutilização de múltiplos atributos de vértice, como posição, cor, normais, etc., facilitando a renderização de cenas complexas. Um VAO encapsula todas as chamadas relacionadas à configuração dos atributos de vértice, o que evita a necessidade de reconfigurá-los a cada vez que você renderiza algo.
  
  Sem um VAO, seria necessário configurar os atributos de vértice manualmente toda vez que quiséssemos desenhar uma nova geometria. Isso é custoso e propenso a erros. Com o VAO, você configura os atributos de vértice uma vez e, depois, basta **"ativar"** o VAO para desenhar as geometrias associadas. Isso melhora a eficiência e a legibilidade do código.

### Como utilizar?

1. **Criação do VAO**: Você cria um VAO com `glGenVertexArrays` e o "vincula" (bind) para começar a armazenar as configurações de atributos:
     ```cpp
     // 1. Criação do VAO
     GLuint vao;
     glGenVertexArrays(1, &vao);

     // 2. Vincula o VAO (agora vamos armazenar as configurações de atributos nele)
     glBindVertexArray(vao);
     ```
   Uma vez vinculado, todas as configurações de vértices que fizer (com VBOs e chamadas `glVertexAttribPointer`) serão armazenadas no VAO.
   
2. **Configuração dos Atributos de Vértice**: Com o VAO vinculado, você associa os atributos de vértice (posição, cor, normais, coordenadas de textura, etc.) usando funções como `glVertexAttribPointer` e `glEnableVertexAttribArray`:
     ```cpp
     //3. Definição dos atributos de vértice (index 0: posição)
     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);

     //4. Definição dos atributos de vértice (index 1: cor)
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
     glEnableVertexAttribArray(1);
     ```
   Essas configurações são armazenadas no VAO.

   A função `glVertexAttribPointer` é usada no OpenGL para especificar o layout dos dados de vértices dentro de um **Vertex Buffer Object (VBO)**. Ela informa ao OpenGL como interpretar os dados armazenados no buffer para associá-los aos atributos de vértice (posição, cor, normais, coordenadas de textura, etc.).
   
   Explicação dos parâmetros da função `glVertexAttribPointer`:
     1. **Índice** `GLuint index`: Este é o índice do atributo de vértice a ser configurado (posição, cor, normais, coordenadas de textura, etc.). Esse índice diz ao OpenGL qual atributo está sendo configurado. Cada atributo do vértice (posição, cor, normais, coordenadas de textura, etc.) tem um índice único, que deve ser habilitado com a função `glEnableVertexAttribArray` e configurado corretamente com `glVertexAttribPointer`.
        + `GLuint index = 0`: O índice `0` refere-se ao **atributo de vértice da posição**. No **vertex shader**, este índice deve estar associado ao atributo de posição (`layout(location = 0)`).
        + `GLuint index = 1`: O índice `1` refere-se ao **atributo de vértice da cor**. No **vertex shader**, este índice deve estar associado ao atributo de posição (`layout(location = 1)`).

     3. **Tamanho** `GLint size`: Indica quantos componentes por vértice serão usados para este atributo.
        + `GLint size = 2`: Indica que cada vértice tem **2 componentes** para a **posição**, que podem ser as coordenadas **x** e **y** (se for 2D). Ou seja, para cada vértice, a posição é representada por dois valores `float`.
        + `GLint size = 3`: Indica que cada vértice tem **3 componentes** para a **cor**, que são normalmente **r (vermelho)**, **g (verde)**, e **b (azul)**. Ou seja, a cor de cada vértice é representada por três valores `float`.

     5. **Tipo** `GLenum type`: Este parâmetro define o tipo de dado que está sendo usado. Aqui, `GL_FLOAT` indica que os dados são do tipo `float`. Outros tipos possíveis incluem `GL_INT`, `GL_BYTE`, etc.
        + `GLenum type = GL_FLOAT`: O tipo de dado dos componentes é `GL_FLOAT`, ou seja, os valores de **posição** e **cor** são do tipo `float` (ponto flutuante).

     7. **Normalizado** `GLboolean normalized`: Este parâmetro especifica se os dados devem ser normalizados (convertidos para o intervalo [0, 1] ou [-1, 1]) ao serem usados.
        + `GLboolean normalized = GL_FALSE`: Como `GL_FALSE`, indica que os valores de **posição** não devem ser normalizados (ou seja, eles serão usados diretamente como `float` em vez de serem mapeados para o intervalo [0, 1] ou [-1, 1]).
        + `GLboolean normalized = GL_FALSE`: Como `GL_FALSE`, os valores de **cor** não serão normalizados. Isso significa que os valores são utilizados diretamente, e espera-se que estejam no intervalo adequado (por exemplo, [0.0, 1.0] para `GL_FLOAT`).

     9. **Stride** `GLsizei stride`: O "stride" é o número de bytes entre dois conjuntos consecutivos de dados. O stride é sempre o mesmo para todos os atributos de um vértice, porque cada vértice tem a mesma estrutura na memória.
        + `GLsizei stride = 5 * sizeof(float)`: O **stride** para a configuração dos atributos de vértice da **posição** e **cor** é `5 * sizeof(float)`, ou seja, há **5 floats por vértice**. Isso indica que os dados de cada vértice ocupa 5 floats consecutivos na memória (2 floats para a **posição** (x, y), 3 floats para a **cor** (r, g, b)).

     11. **Deslocamento** `const GLvoid* pointer`: Este parâmetro indica o deslocamento (offset) dentro do buffer onde os dados para este atributo começam.
         + `const void* pointer = (void*)0`: No atributo de vértice para a **posição**, o valor `(voic*)0` indica que o **offset** é `0`, ou seja, os dados começam no início do buffer (sem deslocamento). Se, por exemplo, os dados da posição começassem após outros dados no buffer, você ajustaria esse valor para o deslocamento correto.
         + `const void* pointer = (void*)(2 * sizeof(float))`: O **offset** é `2 * sizeof(float)`, ou seja, o atributo de **cor** começa **depois dos primeiros 2 floats** (que são os da posição). Isso significa que o primeiro dado de cor do vértice começa no terceiro **float** de cada vértice (após a posição x e y).

   A **primeira chamada** (`glVertexAttribPointer(0, 2, ...)`) configura o **atributo de posição**, que possui 2 componentes `float` (x, y). Esses dados começam no início do buffer (offset 0).

   A **segunda chamada** (`glVertexAttribPointer(1, 3, ...)`) configura o **atributo de cor**, que possui 3 componentes `float` (r, g, b). Esses dados começam após os 2 primeiros floats de cada vértice, ou seja, com um offset de `2 * sizeof(float)`.

3. **Desvinculação**: Você pode "desvincular" o VAO e o VBO quando terminar, embora a desvinculação do VAO seja opcional em muitos casos:
   ```cpp
   // Desvincula o VAO (opcional)
   glBindVertexArray(0);

   // Desvincula o VBO (opcional)
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   ```
4. **Renderização**: Para desenhar, basta "vincular" o VAO com `glBindVertexArray` e realizar a chamada de desenho, como `glDrawArrays` ou `glDrawElements`:
     ```cpp
     // 5. Desenho
     glDrawArrays(GL_TRIANGLES, 0, 3);
     ```
</details>
<details>
  <summary><h3>3. Element Buffer Object (EBO)</h3></summary>
  
  O **Element Buffer Object (EBO)**, também conhecido como **Index Buffer Object (IBO)**, é um buffer especial no OpenGL que armazena índices de vértices. Ele é usado para desenhar primitivas reutilizando vértices, o que otimiza o desempenho e economiza memória. Em vez de duplicar os mesmos vértices para desenhar formas complexas, o EBO permite que você defina um conjunto de vértices e depois use índices para referenciá-los, formando as faces (triângulos, quadrados, etc.) de um objeto.
  
  Sem um EBO, se você quiser desenhar uma forma como um retângulo usando triângulos, você precisaria duplicar algumas posições de vértices, pois cada triângulo precisa de seu próprio conjunto de vértices.
  
   Exemplo (dois triângulos para um retângulo):
   ```plaintext
   Vértices = [V0, V1, V2, V3]
   Triângulos = [V0, V1, V2], [V2, V1, V3]
   ```

   Nesse caso, o vértice `V2` e `V1` são repetidos para o segundo triângulo.

  Com um EBO, você pode reutilizar os mesmos dados de vértices especificando índices que se referem aos vértices existentes, reduzindo a redundância.

   Exemplo:
   ```plaintext
   Vértices = [V0, V1, V2, V3]
   Índices = [0, 1, 2, 2, 1, 3]
   ```

   Aqui, o array `Índices` especifica como conectar os vértices para formar triângulos, evitando a duplicação de vértices. Os índices são números que se referem às posições dos vértices no array de vértices.

  O EBO armazena uma lista de **índices** que apontam para os vértices no **Vertex Buffer Object (VBO)**. Quando você usa um EBO, em vez de passar todos os vértices diretamente para `glDrawArrays`, você passa os índices com `glDrawElements`, e o OpenGL desenha os vértices correspondentes com base nesses índices.

### Como utilizar?

  Primeiro definimos os vértices da primitiva:
  ```cpp
  // Dados de Vértices (VBO)
  float vertices[] = {
       // Posições      // Cores
        0.0f,  1.0f,    1.0f, 0.0f, 0.0f,  // P1 - Vermelho
       -1.0f, -1.0f,    0.0f, 1.0f, 0.0f,  // P2 - Verde
        1.0f, -1.0f,    0.0f, 0.0f, 1.0f   // P3 - Azul
  };
  ```
  
  Depois definimos os índices dos vértices que formam um triângulo:
  ```cpp
  // Dados de Índices (EBO)
  unsigned int indices[] = {
      0, 1, 2
  };
  ```

  Então criamos, vinculamos e configuramos os buffers:
  ```cpp
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // Vincula e configura o EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  ```
  Finalmente desenha a primitiva:
  ```cpp
  // Desenha o triângulo
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  ```
Quando o OpenGL desenha formas, ele usa os índices do EBO para acessar os vértices correspondentes do VBO.

</details>

Esses três objetos trabalham juntos para organizar e otimizar a maneira como os dados de vértices e os índices são enviados para a GPU, garantindo um pipeline de renderização mais eficiente.

Quando você usa VAOs, VBOs e EBOs juntos, o fluxo de trabalho típico é o seguinte:

1. **Criação do VAO**:
   - Um **VAO** é criado para armazenar a configuração de atributos de vértices.

2. **Vinculação e Configuração do VBO**:
   - Você cria um **VBO** e o vincula ao VAO. O **VBO** contém os **dados brutos dos vértices** (posições, cores, normais, etc.).
   - A seguir, você configura os **atributos de vértices** (usando `glVertexAttribPointer`) para indicar ao OpenGL como os dados no VBO estão organizados (por exemplo, quais dados correspondem a posição, cor, etc.).

3. **Vinculação e Configuração do EBO**:
   - Você cria um **EBO** e o vincula ao VAO. O **EBO** contém os **índices** que referenciam os vértices no VBO, definindo como eles serão desenhados.
   - O **EBO** permite que você desenhe objetos complexos reutilizando vértices, desenhando com índices através da função `glDrawElements`.

4. **Renderização**:
   - Ao renderizar, basta **vincular o VAO**. Ele automaticamente ativa o VBO e o EBO vinculados e usa as configurações armazenadas.
   - Você pode usar `glDrawArrays` (se não usar um EBO) ou `glDrawElements` (se usar um EBO) para desenhar os objetos.

![Imagem ilustrativa da relação entre VAO (Vertex Array Object), VBO (Vertex Buffer Object) e EBO (Element Buffer Object).](https://learnopengl.com/img/getting-started/vertex_array_objects_ebo.png)

+ **VAO**: Armazena as configurações de como os vértices e índices estão organizados (como e quais VBOs e EBOs usar).
+ **VBO**: Armazena os dados de vértices (posições, cores, normais, etc.).
+ **EBO**: Armazena os índices que referenciam os vértices no VBO, otimizando a renderização ao permitir a reutilização de vértices.
O VAO encapsula a configuração de como os VBOs e EBOs são usados, tornando a renderização eficiente e simplificando o código.
