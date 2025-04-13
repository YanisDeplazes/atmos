# Web Code

## Frontend

The frontend JavaScript is implemented in full **OOP** to adopt a **modular approach**.
With use of SCSS which is served with Vite with hot reload.
Twig was used as a template engine even tho it is not really that necessary for this project.

### Project Structure

The project follows this **structure** for maintainability:

```
/frontend
│
├── /assets                # Static assets and application logic
│   ├── /js               # JavaScript modules (OOP, ES6+, app logic)
│   ├── /shaders          # WebGL / Three.js shaders
│   └── /styles           # SCSS stylesheets and design utilities
│
├── /templates             # Twig templating system
│   ├── /partials         # Reusable UI components (e.g. header, footer)
│   ├── base.twig         # Base layout used by pages
│   └── index.twig        # Entry point template rendered via PHP
│
├── build-twig.php         # Builds `index.php` using Twig and Vite manifest
├── index.php              # Front controller that renders the main view (Twig entry)
│
├── .eslintrc.json         # JavaScript linting configuration (Airbnb + Prettier)
├── .htmlhintrc            # HTML validation rules
├── .stylelintrc.json      # SCSS/CSS linting rules
├── package.json           # NPM dependencies and Vite build scripts
└── vite.config.js         # Vite dev server and build configuration
```

### Coding Standards

To keep the frontend code **clean, readable, and maintainable**, the following conventions are used:

#### **JavaScript (ESLint & Prettier)**

- **root** (`root: true`): Marks this ESLint configuration as the root configuration, preventing ESLint from searching further up the directory structure.
- **extends** (`extends: ["airbnb-base", "eslint:recommended", "prettier"]`):
  - Inherits rules from:
    - **airbnb-base**: Airbnb's JavaScript style guide (excluding React rules).
    - **eslint:recommended**: Standard recommended rules provided by ESLint.
    - **prettier**: Rules that ensure compatibility with Prettier formatting.
- **plugins** (`plugins: ["prettier"]`): Enables integration with the Prettier formatter.
- **prettier/prettier** (`error`): Ensures code formatting adheres to Prettier configuration.
- **camelcase** (`error`): Enforces camelCase naming convention for identifiers.
- **new-cap** (`error`): Requires constructors to start with an uppercase letter.
- **no-underscore-dangle** (`error`): Disallows identifiers from starting or ending with underscores.
- **prefer-const** (`error`): Enforces using `const` for variables that are never reassigned.
- **eqeqeq** (`error`): Requires strict equality (`===`) over abstract equality (`==`).
- **arrow-body-style** (`["error", "as-needed"]`): Enforces concise arrow function syntax when possible.
- **object-curly-spacing** (`["error", "always"]`): Requires spaces inside curly braces.
- **no-console** (`warn` with exceptions for `warn` and `error`): Allows `console.warn` and `console.error`, but warns against other console methods.

#### HTML

- All attribute names must be in lowercase.
- Elements cannot have duplicate attributes.
- Attribute values cannot contain unsafe chars
- Doctype must be declared first.
- Invalid doctype.
- The `<script>` tag cannot be used in a tag.
- The id and class attribute values must meet the specified rules: "dash"
- The value of id attributes must be unique.
- The alt attribute of an element must be present and alt attribute of area[`href`] and input[`type=image`] must have a value.
- Do not mix tabs and spaces for indentation: "space"
- Special characters must be escaped.
- `<style>` tags cannot be used.
- Tag must be paired.
- Empty tags must be self closed.
- All html element names must be in lowercase.
- `<title>` must be present in `<head>` tag.

#### SCSS

- Specify short or long notation for hex colors: short
- Disallow named colors.
- Limit the depth of nesting: 3
- Pattern for class selectors: `"^[a-z]+([a-z0-9-]+[a-z0-9]+)?$"`
- selector-id-pattern: `"^[a-z]+([a-z0-9-]+[a-z0-9]+)?$"`
- scss/at-mixin-pattern: `"^[a-z]+([a-z0-9-]+[a-z0-9]+)?$"`
- scss/dollar-variable-pattern: `"^[a-z]+([a-z0-9-]+[a-z0-9]+)?$"`
- Disallow extension in @import, @use, @forward, and [meta.load-css] commands.
- Always Require whitespace after the // in //-comments
- Disallow !important within declarations.
- List of disallowed units. `["pt"]`
- Disallow units for zero lengths.
- Disallow vendor prefixes for properties.
- Disallow vendor prefixes for values.
- Disallow selectors of lower specificity from coming after overriding selectors of higher specificity. null
- Disallow empty blocks.
- Require whitespace on the inside of comment markers.
- Disallow qualifying a selector by type. `[true, { "ignore": ["attribute", "class", "id"] }]`

### Linting & Formatting

The project includes **ESLint**, **htmlhint** and **stylelint** for code quality.

#### Run Code Linting

```sh
npm run lint
```

```sh
npm run lint:html
```

```sh
npm run lint:scss
```

#### Fix Formatting Issues

```sh
npm run lint:fix
```

```sh
npm run lint:scss:fix
```

### Build

```sh
npm run build
```

Automatically builds a new build version with vite and twig.

### Testing

Manual UI testing was conducted in Chrome and Firefox. All HTTP responses were verified to ensure proper status codes and JSON formatting.

### Code Documentation

- See the [Detailed Javascript Documentation](./code_frontend.md)

### Repository

- **Source Code:** [GitHub Repository](https://github.com/YanisDeplazes/atmos/tree/main/web/src/frontend?ref_type=heads)

## Backend

The backend is implemented in full OOP in order to build a **RESTful API** with **MVC principles**.

### Project Structure

The project follows this **structure** for maintainability:

```
/api
│── /Routes
│   ├── Api.php            # API Router – handles incoming requests
│── /Controllers
│   ├── Controller.php     # Base controller – handles CRUD logic
│── /Models
│   ├── Model.php          # Generic database model (CRUD operations)
│   ├── Database.php       # Singleton DB connection
│── /Views
│   ├── View.php           # Handles database views
│── index.php              # API entry point
│── composer.json          # Dependencies and autoloading
```

### Coding Standards

To keep the backend code **clean, readable, and maintainable**, the following conventions are used:

- **PSR12 Standard**: Enforces PHP coding style according to the PSR-12 Extended Coding Style standard, including:
  - Proper namespace and class declarations.
  - Correct method and property visibility.
  - Consistent indentation and spacing.
  - Control structures formatting.
  - Adherence to PHPDoc standards.

#### Run Code Linting

```sh
composer lint
```

#### Fix Formatting Issues

```sh
composer fix
```

### Testing

All API endpoints were tested using Postman and automated Jest test cases for frontend logic.

### Deployment

For public PHP API is containerized via Docker, with services for PHP-FPM, Nginx, and MariaDB. For public demos, the API is tunneled using LocalTunnel.

### Code Documentation

- See the [Detailed Code Documentation](./code_backend.md)

### Repository

- **Source Code:** [GitHub Repository](https://github.com/YanisDeplazes/atmos/tree/main/web/src/api?ref_type=heads)
