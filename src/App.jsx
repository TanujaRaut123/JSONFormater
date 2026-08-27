import React from 'react';

export default function App() {
  const [input, setInput] = React.useState('');
  const [output, setOutput] = React.useState('');
  const [error, setError] = React.useState('');
  const [cleanedPreview, setCleanedPreview] = React.useState('');

  const cleanInput = (value) => {
    let cleaned = value || '';
    const backslash = String.fromCharCode(92);
    const doubleQuote = String.fromCharCode(34);
    const escapedNewLine = backslash + 'n';
    const escapedReturn = backslash + 'r';
    const escapedTab = backslash + 't';
    const escapedQuote = backslash + doubleQuote;

    const nonSpaceLength = cleaned.replace(/\s/g, '').length;
    const spaceLength = cleaned.length - nonSpaceLength;
    const looksCharacterSpaced =
      spaceLength > nonSpaceLength * 0.35 &&
      cleaned.includes(backslash + ' ') &&
      cleaned.includes(doubleQuote + ' ');

    if (looksCharacterSpaced) {
      cleaned = cleaned.replace(/\s+/g, '');
    }

    cleaned = cleaned
      .split(escapedNewLine).join(' ')
      .split(escapedReturn).join(' ')
      .split(escapedTab).join(' ')
      .split(escapedQuote).join(doubleQuote)
      .split('&quot;').join(doubleQuote)
      .split('&amp;quot;').join(doubleQuote)
      .replace(/<br\s*\/?\s*>/gi, ' ')
      .replace(/\.\.\./g, '')
      .replace(/&nbsp;/gi, ' ')
      .replace(/\s+/g, ' ')
      .trim();

    const firstBrace = cleaned.indexOf('{');
    const lastBrace = cleaned.lastIndexOf('}');
    if (firstBrace >= 0 && lastBrace > firstBrace) {
      cleaned = cleaned.substring(firstBrace, lastBrace + 1);
    }

    return cleaned;
  };

  const formatJson = () => {
    try {
      const cleaned = cleanInput(input);
      setCleanedPreview(cleaned);
      const parsed = JSON.parse(cleaned);
      setOutput(JSON.stringify(parsed, null, 2));
      setError('');
    } catch (formatError) {
      setError(`Invalid JSON: ${formatError.message}`);
      setOutput('');
    }
  };

  const clearAll = () => {
    setInput('');
    setOutput('');
    setError('');
    setCleanedPreview('');
  };

  const copyOutput = async () => {
    if (!output) return;

    try {
      if (navigator.clipboard && window.isSecureContext) {
        await navigator.clipboard.writeText(output);
      } else {
        const textArea = document.createElement('textarea');
        textArea.value = output;
        textArea.style.position = 'fixed';
        textArea.style.left = '-999999px';
        document.body.appendChild(textArea);
        textArea.focus();
        textArea.select();
        document.execCommand('copy');
        document.body.removeChild(textArea);
      }
    } catch {
      setError('Failed to copy output to clipboard.');
    }
  };

  return (
    <main className="container">
      <h1>JSON Pretty Formatter</h1>
      <p className="description">
        Paste raw JSON or copied log/email payloads. The formatter cleans escaped
        newlines, escaped quotes, HTML br tags, ellipsis artifacts, extra spaces,
        and character-spaced payloads before formatting.
      </p>

      <textarea
        className="input"
        placeholder="Paste JSON here..."
        value={input}
        onChange={(event) => setInput(event.target.value)}
      />

      <div className="actions">
        <button className="button button-primary" onClick={formatJson}>
          Format JSON
        </button>
        <button
          className="button button-success"
          onClick={copyOutput}
          disabled={!output}
        >
          Copy Output
        </button>
        <button className="button button-muted" onClick={clearAll}>
          Clear
        </button>
      </div>

      {error && (
        <section className="error">
          <strong>Formatting Failed</strong>
          <div>{error}</div>
          {cleanedPreview && (
            <details>
              <summary>Show cleaned text used for parsing</summary>
              <pre className="preview">{cleanedPreview}</pre>
            </details>
          )}
        </section>
      )}

      {output && (
        <section>
          <h2>Formatted Output</h2>
          <pre className="output">{output}</pre>
        </section>
      )}
    </main>
  );
}
