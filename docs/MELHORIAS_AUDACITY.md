# Melhorias Inspiradas no Audacity

## Análise do Audacity

### Controles de Transporte (Transport Controls)

**Audacity tem**:
1. ⏮️ **Skip to Start** - Volta ao início
2. ▶️ **Play** - Reproduz
3. ⏸️ **Pause** - Pausa
4. ⏹️ **Stop** - Para
5. ⏭️ **Skip to End** - Vai ao fim
6. 🔁 **Loop** - Toggle de loop

**Atalhos de teclado**:
- `Space` - Play/Pause
- `X` - Stop
- `Shift+Space` - Loop Play
- `Home` - Início
- `End` - Fim

### Seleção de Região

**Audacity tem**:
1. **Selection Toolbar** com caixas de tempo editáveis:
   - Start Time: `00h 00m 00.000s`
   - End Time: `00h 00m 00.000s`
   - Length: `00h 00m 00.000s`
   - Center: `00h 00m 00.000s`

2. **Seleção com mouse**:
   - Clique e arraste na waveform
   - Shift+Clique para estender seleção
   - Ctrl+Clique para mover cursor sem selecionar

3. **Indicadores visuais**:
   - Região selecionada em cinza claro
   - Bordas da seleção em linhas verticais
   - Cursor de reprodução em linha vertical preta

### Timeline

**Audacity tem**:
- Régua de tempo no topo
- Marcadores de tempo principais e secundários
- Formato de tempo configurável (hh:mm:ss, samples, etc.)

## Implementações Planejadas

### ✅ Fase 1: TimeEdit Widget
- [x] Widget de edição de tempo HH:MM:SS.mmm
- [x] Campos editáveis individuais
- [x] Validação automática
- [x] Fonte monoespaçada

### 🔄 Fase 2: Controles de Seleção
- [ ] Selection Toolbar com:
  - Start Time (editável)
  - End Time (editável)
  - Length (calculado automaticamente)
- [ ] Sincronização bidirecional com visualização
- [ ] Botões para limpar seleção

### 🔄 Fase 3: Melhorar Controles de Transporte
- [ ] Botões separados para Play e Pause
- [ ] Botão Skip to Start
- [ ] Botão Skip to End
- [ ] Ícones melhores (estilo Audacity)
- [ ] Feedback visual de estado

### 🔄 Fase 4: Atalhos de Teclado
- [ ] Space - Play/Pause
- [ ] X - Stop
- [ ] Home - Início
- [ ] End - Fim
- [ ] Shift+A - Selecionar tudo
- [ ] Ctrl+1 - Zoom in
- [ ] Ctrl+3 - Zoom out

### 🔄 Fase 5: Melhorar Visualização
- [ ] Cursor de reprodução mais visível
- [ ] Seleção com cor de fundo
- [ ] Bordas de seleção destacadas
- [ ] Régua de tempo no topo
- [ ] Snap to grid (opcional)

## Código de Referência do Audacity

### Transport Controls
```cpp
// src/toolbars/ControlToolBar.cpp
void ControlToolBar::OnPlay(wxCommandEvent & WXUNUSED(evt))
{
    auto &projectAudioManager = ProjectAudioManager::Get( mProject );
    
    if (projectAudioManager.Playing())
        projectAudioManager.Stop();
    else
        projectAudioManager.PlayCurrentRegion();
}
```

### Selection Toolbar
```cpp
// src/toolbars/SelectionBar.cpp
void SelectionBar::SetTimes(double start, double end)
{
    mStart = start;
    mEnd = end;
    mLength = end - start;
    mCenter = (start + end) / 2.0;
    
    UpdateDisplay();
}
```

### Cursor Drawing
```cpp
// src/TrackArt.cpp
void DrawCursor(wxDC &dc, const wxRect &rect, double time)
{
    dc.SetPen(*wxBLACK_PEN);
    int x = TimeToPosition(time, rect);
    dc.DrawLine(x, rect.y, x, rect.y + rect.height);
}
```

## Prioridades

### Alta Prioridade (Implementar Agora)
1. ✅ TimeEdit widget
2. 🔄 Selection Toolbar (Start/End/Length)
3. 🔄 Botões Play/Pause separados
4. 🔄 Atalhos de teclado básicos (Space, X)

### Média Prioridade (Próxima Iteração)
1. Skip to Start/End
2. Melhorar cursor de reprodução
3. Melhorar visualização de seleção
4. Régua de tempo

### Baixa Prioridade (Futuro)
1. Snap to grid
2. Múltiplos formatos de tempo
3. Marcadores/labels
4. Zoom to selection

## Diferenças Aceitáveis

**Audacity** é um editor de áudio completo.
**BioEcho** é um anotador de áudio.

**Não precisamos**:
- Edição de áudio (cortar, copiar, colar)
- Efeitos de áudio
- Multi-track
- Geração de áudio
- Análise espectral avançada

**Precisamos focar em**:
- Reprodução confiável
- Seleção precisa de regiões
- Loop perfeito
- Anotação eficiente
- Navegação rápida

## Status Atual

### ✅ Já Temos
- Reprodução com PortAudio (latência mínima)
- Loop de região funcionando
- Seleção com Shift+Drag
- Ctrl+Click para posicionar
- Visualização de waveform
- Zoom e pan

### 🔄 Em Desenvolvimento
- TimeEdit widget (criado)
- Selection Toolbar (próximo)
- Botões melhorados (próximo)

### ⏳ Planejado
- Atalhos de teclado
- Melhorias visuais
- Régua de tempo
