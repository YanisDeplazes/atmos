<?php
/**
 * Twig Static Builder Script
 *
 * Renders the `index.twig` template using production-ready Vite assets
 * by reading from the generated Vite manifest. Outputs the resulting HTML
 * to the build directory as `index.php`.
 *
 * @author Yanis Deplazes
 * @date 2025-04-09
 */

require_once __DIR__ . '/vendor/autoload.php';

use Twig\Environment;
use Twig\Loader\FilesystemLoader;

$loader = new FilesystemLoader(__DIR__ . '/templates');
$twig = new Environment($loader);

$outputDir = __DIR__ . '/../../build/frontend';

if (!is_dir($outputDir)) {
    mkdir($outputDir, 0755, true);
}

// Adjusted manifest path
$manifestPath = __DIR__ . '/../../build/frontend/.vite/manifest.json';
$jsScript = '/assets/main.js'; // fallback
$cssLink = ''; // optional, fallback empty

if (file_exists($manifestPath)) {
    $manifest = json_decode(file_get_contents($manifestPath), true);

    $entry = 'assets/js/main.js';
    if (isset($manifest[$entry]['file'])) {
        $jsScript = './' . $manifest[$entry]['file'];
    }

    if (isset($manifest[$entry]['css'][0])) {
        $cssLink = './' . $manifest[$entry]['css'][0];
    }
}

// Template data
$data = [
    'title' => 'IoT Data Dashboard',
    'project_name' => 'ATMOS',
    'settings_label' => 'Settings',
    'author_name' => 'Yanis Deplazes',
    'author_link' => 'https://yanis.io',
    'year' => '2025',
    'js_script' => $jsScript,
    'css_link' => $cssLink,
];

$html = $twig->render('index.twig', $data);

file_put_contents($outputDir . '/index.php', $html);

echo "✅ index.php built with:\n";
echo "   → JS: $jsScript\n";
echo "   → CSS: $cssLink\n";
